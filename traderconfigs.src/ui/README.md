# TraderConfigs UI - Interfaz Visual

Una interfaz web moderna para visualizar, editar y ejecutar configuraciones de componentes.

## Cómo usar

### Opción 1: Doble clic (Fácil) ✨
1. Ve a la carpeta raíz del proyecto: `c:\Users\Alex\Desktop\Trader`
2. Doble clic en **`iniciar_ui.bat`**
3. Se abrirá automáticamente en tu navegador en http://127.0.0.1:5000

### Opción 2: Desde PowerShell (Manual)
```powershell
cd c:\Users\Alex\Desktop\Trader\traderconfigs.src\ui
C:/Users/Alex/AppData/Local/Python/pythoncore-3.14-64/python.exe app.py
```
Luego abre en navegador: http://127.0.0.1:5000

## Interfaz

### Panel izquierdo: Componentes Candidatos
- Muestra el contenido del archivo `Candidatos.txt`
- Puedes editar directamente en el textarea

### Panel derecho: Arquitectura
- Muestra el contenido del archivo `Arquitectura.txt`
- Editable también

### Botón "Ejecutar análisis"
- Ejecuta TraderConfigs.exe con los datos actuales
- Muestra el número total de configuraciones válidas

### Tabla de resultados
- **ID**: número de configuración
- **Servicios seleccionados**: lista de componentes (Cx.Servicio)
- **Total**: cantidad de componentes en esa solución

## Requisitos

✅ Python 3.14+ (ya instalado)
✅ Flask 3.1+ (ya instalado)
✅ TraderConfigs.exe (generado con compilar.bat)
✅ Navegador web (Chrome, Edge, Firefox, etc.)

## Detener el servidor

- Cierra la ventana de PowerShell/CMD donde se ejecuta
- O presiona Ctrl+C en el terminal

## Notas

- Los cambios en los textareas **no** guardan automáticamente en el disco
- Cada vez que ejecutas, se usan los textos actuales del textarea
- Los resultados se muestran en una tabla con scroll horizontal
