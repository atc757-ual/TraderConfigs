from pathlib import Path
import re
import subprocess
import tempfile
from flask import Flask, jsonify, render_template, request
import os
import shutil

app = Flask(__name__, template_folder='templates', static_folder='static')

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PROJECT_ROOT = Path(BASE_DIR)
EXE_PATH = PROJECT_ROOT / "TraderConfigs.exe"
CANDIDATES_FILE = PROJECT_ROOT / "ejemplos" / "Candidatos.txt"
ARCH_FILE = PROJECT_ROOT / "ejemplos" / "Arquitectura.txt"
SINGLE_OUT = PROJECT_ROOT / "single_configurations.out"
LONG_OUT = PROJECT_ROOT / "long_configurations.out"


def get_env_with_mingw():
    """Retorna env con PATH incluyendo MinGW"""
    env = os.environ.copy()
    # Agregar rutas comunes de MinGW
    mingw_paths = [
        r"C:\Program Files\mingw-w64\x86_64-8.1-posix-seh-rt_v6-rev0\mingw64\bin",
        r"C:\Program Files (x86)\mingw-w64\i686-8.1-posix-dwarf-rt_v6-rev0\mingw32\bin",
        r"C:\Program Files\Git\mingw64\bin",
        r"C:\msys64\mingw64\bin",
    ]
    
    # Se agregan las rutas existentes de PATH System y User
    machine_path = os.environ.get("PATH", "")
    
    # Crear nuevo PATH con MinGW primero
    for mingw_path in mingw_paths:
        if os.path.exists(mingw_path):
            env["PATH"] = mingw_path + ";" + machine_path
            return env
    
    # Si no encuentra mingw, retornar env sin cambios
    return env


def parse_architecture(arch_text: str) -> dict:
    """Extrae servicios válidos y dependencias de la arquitectura"""
    valid_services = set()  # Servicios que la arquitectura OFRECE (O:)
    dependencies = {}  # servicio -> [servicios que requiere (I:)]
    output_groups = []  # Grupos de servicios ofrecidos juntos por componente en arquitectura
    
    lines = arch_text.strip().split('\n')
    for line in lines:
        line = line.strip()
        if not line:
            continue
        
        # Extraer servicios ofrecidos (O:)
        offered_in_line = []
        if 'O:' in line:
            parts = line.split('I:')[0]
            o_idx = parts.find('O:')
            if o_idx != -1:
                services_str = parts[o_idx+2:].strip()
                for svc in services_str.split():
                    if svc:
                        valid_services.add(svc)
                        offered_in_line.append(svc)

        if offered_in_line:
            output_groups.append(set(offered_in_line))
        
        # Extraer dependencias (I:) para cada servicio ofrecido en esta línea
        if 'I:' in line and offered_in_line:
            i_idx = line.find('I:')
            services_str = line[i_idx+2:].strip()
            required = []
            for svc in services_str.split():
                if svc:
                    required.append(svc)
            
            # Cada servicio ofrecido en esta línea requiere estos servicios
            for offered_svc in offered_in_line:
                if offered_svc not in dependencies:
                    dependencies[offered_svc] = set()
                dependencies[offered_svc].update(required)
    
    # Determinar si la arquitectura sigue el patrón de un servicio por componente
    single_output_per_component = all(len(group) == 1 for group in output_groups)

    return {
        "valid_services": valid_services,
        "dependencies": dependencies,
        "output_groups": output_groups,
        "single_output_per_component": single_output_per_component,
    }


def parse_components_architecture(candidates_text: str) -> dict:
    """Extrae los servicios ofrecidos por CADA componente"""
    all_services = set()  # Todos los servicios ofrecidos por los candidatos
    component_count = 0
    component_interfaces = {}  # {C1: {offered:set(), required:set()}, ...}
    
    lines = candidates_text.strip().split('\n')
    
    for line in lines:
        line = line.strip()
        if not line:
            continue
        
        # Extraer servicios ofrecidos en este componente
        if 'O:' in line:
            component_count += 1
            comp_id = f"C{component_count}"
            offered = set()
            required = set()

            parts = line.split('I:')[0]
            o_idx = parts.find('O:')
            if o_idx != -1:
                services_str = parts[o_idx+2:].strip()
                for svc in services_str.split():
                    if svc:
                        all_services.add(svc)
                        offered.add(svc)

            if 'I:' in line:
                i_idx = line.find('I:')
                req_str = line[i_idx+2:].strip()
                for svc in req_str.split():
                    if svc:
                        required.add(svc)

            component_interfaces[comp_id] = {
                "offered": offered,
                "required": required,
            }

    return {
        "all_services": all_services,
        "component_count": component_count,
        "component_interfaces": component_interfaces,
    }


def validate_configuration(
    config_services: list,
    arch_valid: set,
    arch_dependencies: dict,
    arch_output_groups: list,
    arch_single_output: bool,
    component_interfaces: dict,
) -> tuple[bool, bool, dict]:
    """
    Valida si una configuración respeta arquitectura y está cerrada.
    
    Completitud (respeta):
    - Tiene TODOS los servicios requeridos por la arquitectura
    - NO viola la estructura de dependencias (un componente no puede ofrecer un servicio Y sus dependencias)
    - NO hay fragmentación (cada servicio viene de UN SOLO componente)
    - Puede tener servicios extra (composición)
    
    Cerrada (clausura):
    - NO tiene servicios intrusos (todo lo ofrecido pertenece a la arquitectura)
    - Es independiente de respeta
    """
    # Extraer servicios ofrecidos por cada componente en la configuración
    component_services = {}  # {C1: [svc1, svc2], C2: [...]}
    service_to_components = {}  # {servicio: [C1, C2, ...]} para detectar fragmentación
    config_offered = set()
    
    for svc in config_services:
        if "." in svc:
            comp, service_name = svc.split(".", 1)
            config_offered.add(service_name)
            
            if comp not in component_services:
                component_services[comp] = []
            component_services[comp].append(service_name)
            
            # Track en qué componentes aparece cada servicio
            if service_name not in service_to_components:
                service_to_components[service_name] = []
            service_to_components[service_name].append(comp)
    
    # Verificar estructura de dependencias:
    # Si un servicio S requiere un servicio D (según arquitectura),
    # entonces S y D NO deben venir del mismo componente
    violates_structure = False
    for comp, services in component_services.items():
        for svc in services:
            if svc in arch_dependencies:
                required_by_svc = arch_dependencies[svc]
                # Verificar si este componente también ofrece alguna dependencia
                for dep in required_by_svc:
                    if dep in services:
                        # Violación: el componente ofrece tanto el servicio como su dependencia
                        violates_structure = True
                        break
            if violates_structure:
                break

    # Verificar unificación respecto de la arquitectura:
    # si dos servicios están en grupos de salida distintos de arquitectura,
    # no deben aparecer juntos en el mismo componente candidato.
    service_to_group_ids = {}
    for idx, group in enumerate(arch_output_groups):
        for service in group:
            if service not in service_to_group_ids:
                service_to_group_ids[service] = set()
            service_to_group_ids[service].add(idx)

    violates_arch_output_separation = False
    for services in component_services.values():
        unique_services = sorted(set(services))
        if len(unique_services) < 2:
            continue

        for i in range(len(unique_services)):
            for j in range(i + 1, len(unique_services)):
                s1 = unique_services[i]
                s2 = unique_services[j]

                groups_1 = service_to_group_ids.get(s1, set())
                groups_2 = service_to_group_ids.get(s2, set())

                if not groups_1 or not groups_2:
                    continue

                if groups_1.isdisjoint(groups_2):
                    violates_arch_output_separation = True
                    break

            if violates_arch_output_separation:
                break

        if violates_arch_output_separation:
            break

    # Verificar si un candidato ofrece múltiples servicios cuando arquitectura tiene patrón de salida única
    violates_single_output_pattern = False
    if arch_single_output:
        for services in component_services.values():
            unique_services = set(services)
            if len(unique_services) > 1:
                violates_single_output_pattern = True
                break

    # Verificar fragmentación:
    # Cada servicio debe venir de UN SOLO componente
    has_fragmentation = False
    for service, components in service_to_components.items():
        if len(components) > 1:
            # Fragmentación detectada: este servicio viene de múltiples componentes
            has_fragmentation = True
            break

    # Ruido en O: se ofrece algo fuera de arquitectura
    has_noise_in_o = not config_offered.issubset(arch_valid)

    # Ruido en I: algún componente seleccionado requiere servicios fuera de arquitectura
    has_noise_in_i = False
    for comp in component_services:
        comp_req = component_interfaces.get(comp, {}).get("required", set())
        if not set(comp_req).issubset(arch_valid):
            has_noise_in_i = True
            break
    
    # C (cerrada):
    # - tiene TODOS los servicios de la arquitectura
    # - sin ruido en O
    # - sin ruido en I
    has_all_services = arch_valid.issubset(config_offered)
    cerrada = has_all_services and not has_noise_in_o and not has_noise_in_i

    # R (respeta):
    # - NO unifica en un mismo componente servicios que la arquitectura separa
    # - NO ofrece múltiples servicios si arquitectura usa patrón de salida única
    unifies_independent_services = violates_structure or violates_arch_output_separation or violates_single_output_pattern
    respeta = not unifies_independent_services

    respeta_reason = "No unifica outputs independientes en el mismo componente."
    if not respeta:
        if violates_single_output_pattern:
            respeta_reason = "Unifica múltiples servicios cuando arquitectura define una salida por componente."
        elif violates_arch_output_separation:
            respeta_reason = "Unifica servicios que en arquitectura están en componentes separados."
        else:
            respeta_reason = "Unifica outputs independientes en al menos un componente."

    reasons = {
        "respeta": respeta_reason,
        "cerrada": "Todos los inputs requeridos están presentes y no hay ruido." if cerrada else "Faltan inputs requeridos o hay ruido en servicios respecto a la arquitectura.",
        "details": {
            "hasAllServices": has_all_services,
            "violatesStructure": violates_structure,
            "violatesArchOutputSeparation": violates_arch_output_separation,
            "violatesSingleOutputPattern": violates_single_output_pattern,
            "hasFragmentation": has_fragmentation,
            "hasNoiseInO": has_noise_in_o,
            "hasNoiseInI": has_noise_in_i,
            "unifiesIndependentServices": unifies_independent_services,
            "requiredOutsideArchitecture": has_noise_in_i,
        },
    }

    return respeta, cerrada, reasons


def read_text_or_empty(path: Path) -> str:
    if not path.exists():
        return ""
    return path.read_text(encoding="utf-8", errors="replace")


def parse_single_output(path: Path, candidates_text: str = "", architecture_text: str = "") -> list[dict]:
    if not path.exists():
        return []

    # Parsear arquitectura para obtener servicios válidos y dependencias
    arch_info = parse_architecture(architecture_text) if architecture_text else {"valid_services": set(), "dependencies": {}, "output_groups": [], "single_output_per_component": False}
    arch_valid = arch_info["valid_services"]
    arch_dependencies = arch_info["dependencies"]
    arch_output_groups = arch_info["output_groups"]
    arch_single_output = arch_info["single_output_per_component"]
    
    # Parsear componentes para obtener todos los servicios disponibles
    components_arch = parse_components_architecture(candidates_text) if candidates_text else {}
    component_interfaces = components_arch.get("component_interfaces", {})

    rows: list[dict] = []
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        match = re.match(r"^\s*(\d+):\s*(.*)$", line)
        if not match:
            continue

        config_id = int(match.group(1))
        raw = match.group(2).strip()
        services = [item for item in raw.split() if item]
        
        # Agrupar servicios por componente
        components = {}
        for svc in services:
            if "." in svc:
                comp, service = svc.split(".", 1)
                if comp not in components:
                    components[comp] = []
                components[comp].append(service)
        
        rows.append(
            {
                "id": config_id,
                "components": components,
                "respeta": False,
                "cerrada": False,
                "respetaReason": "Sin evaluar",
                "cerradaReason": "Sin evaluar",
            }
        )

    for row in rows:
        config_services = []
        for comp, offered in row["components"].items():
            for service in offered:
                config_services.append(f"{comp}.{service}")

        respeta, cerrada, reasons = validate_configuration(
            config_services,
            arch_valid,
            arch_dependencies,
            arch_output_groups,
            arch_single_output,
            component_interfaces,
        )

        row["respeta"] = respeta
        row["cerrada"] = cerrada
        row["respetaReason"] = reasons["respeta"]
        row["cerradaReason"] = reasons["cerrada"]

    return rows


@app.get("/")
def index():
    return render_template(
        "index.html",
        candidates_content=read_text_or_empty(CANDIDATES_FILE),
        architecture_content=read_text_or_empty(ARCH_FILE),
    )


@app.post("/api/run")
def run_configs():
    payload = request.get_json(silent=True) or {}
    candidates_text = payload.get("candidates", "")
    architecture_text = payload.get("architecture", "")

    if not candidates_text.strip() or not architecture_text.strip():
        return jsonify({"ok": False, "error": "Debes cargar contenido en ambos archivos."}), 400

    architecture_info = parse_architecture(architecture_text)
    if not architecture_info.get("valid_services"):
        return jsonify(
            {
                "ok": False,
                "error": "Arquitectura inválida: no se detectaron servicios en O:.",
            }
        ), 400

    if not EXE_PATH.exists():
        return jsonify(
            {
                "ok": False,
                "error": f"No existe TraderConfigs.exe en: {EXE_PATH}",
            }
        ), 400

    # Crear archivos temp y output en el directorio del proyecto
    tmp_candidates = PROJECT_ROOT / f"_tmp_candidates_{id(request)}.txt"
    tmp_architecture = PROJECT_ROOT / f"_tmp_architecture_{id(request)}.txt"
    tmp_single_out = PROJECT_ROOT / f"_tmp_single_{id(request)}.out"
    tmp_long_out = PROJECT_ROOT / f"_tmp_long_{id(request)}.out"
    
    try:
        tmp_candidates.write_text(candidates_text, encoding="utf-8")
        tmp_architecture.write_text(architecture_text, encoding="utf-8")

        try:
            # Ejecutar desde PROJECT_ROOT - el exe genera output ahí
            completed = subprocess.run(
                [str(EXE_PATH), str(tmp_candidates), str(tmp_architecture)],
                cwd=str(PROJECT_ROOT),
                capture_output=True,
                text=True,
                timeout=60,
                env=get_env_with_mingw(),
            )
        except subprocess.TimeoutExpired:
            return jsonify({"ok": False, "error": "La ejecución excedió el tiempo límite (60s)."}), 500
        except Exception as e:
            return jsonify({"ok": False, "error": f"Error al ejecutar: {str(e)}"}), 500

        # El exe genera el output como "single_configurations.out" en PROJECT_ROOT
        # Copiar a archivos temporales (dejar originales visibles)
        exe_generated = PROJECT_ROOT / "single_configurations.out"
        if exe_generated.exists():
            shutil.copy2(exe_generated, tmp_single_out)
        
        exe_long = PROJECT_ROOT / "long_configurations.out"
        if exe_long.exists():
            shutil.copy2(exe_long, tmp_long_out)

        rows = parse_single_output(tmp_single_out, candidates_text, architecture_text)
        long_rows = []
        long_exists = tmp_long_out.exists()
        if long_exists:
            long_rows = parse_single_output(tmp_long_out, candidates_text, architecture_text)
        
        component_count = parse_components_architecture(candidates_text).get("component_count", 0)
        
        success = tmp_single_out.exists() and len(rows) > 0

        return jsonify(
            {
                "ok": success,
                "returnCode": completed.returncode,
                "stdout": completed.stdout,
                "stderr": completed.stderr,
                "totalRows": len(rows),
                "componentCount": component_count,
                "rows": rows,
                "longOutExists": long_exists,
                "longRows": long_rows,
                "totalLongRows": len(long_rows),
            }
        )
    finally:
        # Limpiar archivos temporales
        for path in [tmp_candidates, tmp_architecture, tmp_single_out, tmp_long_out]:
            try:
                path.unlink()
            except:
                pass


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=False)