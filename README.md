# TraderConfigs

## Descripción

TraderConfigs es una herramienta de análisis de configuraciones de componentes que permite evaluar todas las combinaciones posibles de servicios ofrecidos y requeridos, generando un conjunto óptimo de soluciones.

El proyecto incluye tanto una interfaz de línea de comandos (CLI) como una moderna interfaz web construida con Flask, HTML5 y JavaScript.

## 🚀 Inicio Rápido

### Opción 1: Interfaz Web (Recomendado) ✨
1. Haz doble clic en **`iniciar_ui.bat`** en la carpeta raíz
2. Se abrirá automáticamente en tu navegador en http://127.0.0.1:5000

### Opción 2: CLI desde PowerShell
```powershell
.\compilar.bat                    # Compila TraderConfigs.exe
cd .\traderconfigs.src
.\TraderConfigs.exe               # Ejecuta el análisis
```

### Opción 3: Interfaz Web Manual
```powershell
cd .\traderconfigs.src\ui
& ..\..\.venv\Scripts\python.exe .\app.py
```
Luego abre en navegador: http://127.0.0.1:5000

## 📁 Estructura del Proyecto

```
Trader/
├── compilar.bat                      # Compila TraderConfigs.exe
├── iniciar_ui.bat                    # Inicia el servidor web
├── README.md                         # Este archivo
├── .venv/                            # Entorno virtual Python
└── traderconfigs.src/
    ├── TraderConfigs                 # Ejecutable compilado
    ├── TraderConfigs.exe             # (generado por compilar.bat)
    ├── Makefile.win                  # Configuración de compilación
    ├── codigo/
    │   ├── TraderConfigs.cpp/h       # Lógica principal
    │   ├── component.cpp/h           # Modelos de componentes
    │   ├── operations.cpp/h          # Operaciones del análisis
    │   └── error.cpp/h               # Manejo de errores
    ├── ejemplos/
    │   ├── Arquitectura.txt           # Ejemplo de arquitectura
    │   └── Candidatos.txt            # Ejemplo de componentes candidatos
    └── ui/
        ├── app.py                    # Servidor Flask
        ├── README.md                 # Documentación de la UI
        ├── templates/
        │   └── index.html            # Interfaz principal
        └── static/
            └── styles.css            # Estilos CSS
```

## 🎨 Interfaz Web v2.0 - Características

### Panel Arquitectura
- Visualización de la arquitectura actual
- Botón "Traer ejemplo" para cargar arquitectura de prueba
- Tabla de componentes interactiva
- Botón "Limpiar todo" (visible solo con datos)

### Panel Componentes Candidatos
- Gestión de componentes candidatos
- Botón "Traer ejemplo" independiente
- Modal para añadir nuevos componentes
- Validación: tipo (Ofrecido/Requerido) y nombre
- Tabla con opciones de edición/eliminación
- Botón "Limpiar todo" (visible solo con datos)

### Características Adicionales
- **Persistencia local**: Los datos se guardan automáticamente en localStorage
- **Interfaz dual**: Visualización en textarea y tabla intercambiable
- **Validación automática**: El botón "Añadir" se desactiva hasta completar el servicio ofrecido
- **Estilos modernos**: Interfaz responsive con flexbox y diseño card-based
- **Confirmación en acciones destructivas**: Diálogos de confirmación para limpiar datos

## 📋 Requisitos

- **Sistema Operativo**: Windows 10+
- **Python**: 3.7+ (incluido .venv)
- **Compilador C++**: MinGW (para compilar TraderConfigs.exe)
- **Navegador**: Chrome, Edge, Firefox o similar (para la interfaz web)
- **Framework**: Flask 3.1+

## 💻 Desarrollo

### Configurar el ambiente
```powershell
# Activar entorno virtual
.\.venv\Scripts\Activate.ps1

# Instalar dependencias (si es necesario)
pip install flask
```

### Ejecutar el servidor en modo desarrollo
```powershell
cd .\traderconfigs.src\ui
python app.py
```

## 📝 Formatos de Entrada

### Archivo Arquitectura.txt
```
Componente.Servicio1
Componente.Servicio2
Componente.Servicio3
```

### Archivo Candidatos.txt
```
ComponenteCandidato1.ServicioOfrecido: ServicioRequerido1; ServicioRequerido2
ComponenteCandidato2.ServicioOfrecido: ServicioRequerido3
```

## 🔧 API Endpoints

- `GET /` - Interfaz web principal
- `POST /api/run` - Ejecuta el análisis con los datos actuales

## 👥 Autores

- **Luis Iribarne** - Desarrollo y diseño original del motor TraderConfigs
- **Alex Taquila Camasca** - Desarrollo de la interfaz web UI v2.0, features v2.0, localStorage improvements

## 📄 Licencia

Los cambios y mejoras de esta versión 2.0 mantienen consistencia con el proyecto TraderConfigs original.

## 🐛 Solución de Problemas

### El servidor no inicia
- Verifica que el puerto 5000 esté disponible
- Consulta si hay otro proceso ocupando el puerto 5000
- Ejecuta `compilar.bat` primero para generar TraderConfigs.exe

### Los datos no persisten
- Los datos se guardan en localStorage del navegador
- Borra el caché del navegador si hay problemas (Ctrl+Shift+R)
- Los cambios en textarea se guardan automáticamente

### La aplicación se ve rota visualmente
- Presiona Ctrl+Shift+R para limpiar el caché
- Verifica que los archivos CSS se carguen correctamente (F12 → Network)

## 📚 Documentación Adicional

- Ver [traderconfigs.src/ui/README.md](traderconfigs.src/ui/README.md) para detalles de la interfaz web
- Ver [traderconfigs.src/README](traderconfigs.src/README) para información de compilación CLI
