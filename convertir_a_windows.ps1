# Script para convertir archivos a formato Windows (CRLF)
$carpeta = "c:\Users\Alex\Desktop\Trader\traderconfigs.src"

Get-ChildItem -Path "$carpeta\codigo\*" -Include "*.cpp", "*.h" | ForEach-Object {
    Write-Host "Procesando: $_"
    $contenido = [System.IO.File]::ReadAllText($_.FullName, [System.Text.Encoding]::UTF8)
    # Cambiar LF a CRLF (solo si no lo están ya)
    $contenido = $contenido -replace "`r`n", "`n" # Primero normalizar a LF
    $contenido = $contenido -replace "`n", "`r`n" # Luego convertir a CRLF
    [System.IO.File]::WriteAllText($_.FullName, $contenido, [System.Text.Encoding]::UTF8)
}

Write-Host "Conversión completada"
