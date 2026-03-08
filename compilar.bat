@echo off
REM Script para compilar TraderConfigs en Windows
REM Requiere MinGW (g++) o Visual Studio C++ instalado

setlocal enabledelayedexpansion

cd /d "%~dp0\traderconfigs.src\codigo"

REM Variables del compilador
set CC=g++
set CFLAGS=-Wall -std=c++11 -O2
set SOURCES=component.cpp error.cpp operations.cpp TraderConfigs.cpp
set OUTPUT=..\TraderConfigs.exe

echo.
echo ====================================
echo Compilando TraderConfigs para Windows
echo ====================================
echo.

REM Verificar si g++ está disponible
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: g++ no encontrado. 
    echo Por favor, instala MinGW o GCC para Windows.
    echo Descarga desde: https://www.mingw-w64.org/
    pause
    exit /b 1
)

echo Compilador: !CC!
echo Modo: Release
echo.
echo Compilando...

%CC% %CFLAGS% %SOURCES% -o %OUTPUT%

if %errorlevel% neq 0 (
    echo.
    echo ERROR: La compilacion fallo.
    pause
    exit /b 1
)

echo.
echo ====================================
echo Compilacion completada con exito!
echo Ejecutable: %OUTPUT%
echo ====================================
echo.

pause
