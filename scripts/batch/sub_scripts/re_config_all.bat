@echo off
setlocal enabledelayedexpansion

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

@REM ? Delete all directories
call scripts\batch\delete_all.bat

@REM ? Save new build data
echo Saving new build data to %jsonFilePath%
call scripts\batch\sub_scripts\create_build_data.bat "%jsonFilePath%" "%config%" "%generator%"

@REM ! CONNECTOR

@REM ? Configuring SDL2_connector build
echo Configuring SDL2_connector build
call scripts\batch\sub_scripts\cmake_config.bat "SDL2_connector\build" "%config%" "%generator%"

@REM ? Building SDL2_connector
echo Building SDL2_connector
call scripts\batch\sub_scripts\cmake_build.bat "SDL2_connector\build" "%config%"

@REM ! GAME

@REM ? Configuring game build
echo Configuring game build
call scripts\batch\sub_scripts\cmake_config.bat "build" "%config%" "%generator%"

endlocal
