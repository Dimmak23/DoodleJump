@echo off
setlocal enabledelayedexpansion

@REM ? Set up parameters
set "config=%~1"
set "generator=%~2"

@REM ? Check if the required parameter is provided
if "%config%"=="" (
    echo Usage: %~nx0 ^<config^> [generator]
    exit /b 1
)

@REM ? Define the directory path
set "directoryPath=executables"

@REM ? Delete the 'executables' directory
call scripts\batch\sub_scripts\delete.bat "%directoryPath%"

echo Using generator: '%generator%', and config: '%config%'

if "%generator%"=="MinGW Makefiles" (
    set "directoryPath=executables\!config!"

    if not exist "%directoryPath%" (
        echo It's Makefiles generator: bringing back dummy '!directoryPath!'...
        mkdir "!directoryPath!"
    )
)

@REM ? Build using CMake
echo Running CMake build for config: %config%
call scripts\batch\sub_scripts\cmake_build.bat "build" "%config%"

endlocal
