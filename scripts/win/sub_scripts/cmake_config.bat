@echo off
setlocal enabledelayedexpansion

@REM ? Set up parameters
set "directoryPath=%~1"
set "config=%~2"
set "generator=%~3"

@REM ? Check if required parameters are provided
if "%directoryPath%"=="" (
    echo Usage: %~nx0 ^<directoryPath^> ^<config^> [generator]
    exit /b 1
)

if "%config%"=="" (
    echo Usage: %~nx0 ^<directoryPath^> ^<config^> [generator]
    exit /b 1
)

@REM ? Create the directory if it doesn't exist
if not exist "%directoryPath%" (
    mkdir "%directoryPath%"
    echo Directory created: %directoryPath%
) else (
    echo Directory already exists: %directoryPath%
)

@REM ? Navigate to the directory
cd /d "%directoryPath%" || (
    echo Failed to navigate to directory: %directoryPath%
    exit /b 1
)

@REM ? Build type configuration
set "configOption=-DCMAKE_BUILD_TYPE=!config!"

@REM ? Generator option code
set "generatorOption=-G !generator!"

@REM TODO: add support for clang compilator
cmake .. -Wno-dev "!configOption!" "!generatorOption!"

endlocal
