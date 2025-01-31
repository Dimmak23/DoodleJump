@echo off
setlocal

@REM ? Set up parameters
set "jsonFilePath=%~1"
set "config=%~2"
set "generator=%~3"

@REM ? Check if required parameters are provided
if "%jsonFilePath%"=="" (
    echo Usage: %~nx0 ^<jsonFilePath^> ^<config^> ^<generator^>
    exit /b 1
)

if "%config%"=="" (
    echo Usage: %~nx0 ^<jsonFilePath^> ^<config^> ^<generator^>
    exit /b 1
)

if "%generator%"=="" (
    echo Usage: %~nx0 ^<jsonFilePath^> ^<config^> ^<generator^>
    exit /b 1
)

@REM ? Create a JSON string and write it to the file
(
    echo {
    echo     "Config": "%config%",
    echo     "Generator": "%generator%"
    echo }
) > "%jsonFilePath%"

echo JSON data written to %jsonFilePath%

endlocal
