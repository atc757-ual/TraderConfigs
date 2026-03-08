@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0\traderconfigs.src\ui"

REM Ruta permanente al Python instalado
set PYTHON_EXE=C:\Users\Alex\AppData\Local\Python\pythoncore-3.14-64\python.exe

if not exist "!PYTHON_EXE!" (
  echo ERROR: Python no encontrado en: !PYTHON_EXE!
  pause
  exit /b 1
)

echo.
echo ====================================
echo Iniciando TraderConfigs UI...
echo ====================================
echo.
echo Se abrira en: http://127.0.0.1:5000
echo Cierra esta ventana para detener el servidor.
echo.

timeout /t 2 /nobreak

REM Abrir navegador automaticamente
start http://127.0.0.1:5000

REM Ejecutar el servidor
"!PYTHON_EXE!" app.py
