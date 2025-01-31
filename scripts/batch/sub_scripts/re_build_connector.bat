@echo off
setlocal enabledelayedexpansion

@REM ? Set up parameters
set "config=%~1"
set "generator=%~2"

@REM ? Check if required parameters are provided
if "%config%"=="" (
    echo Usage: %~nx0 ^<config^> ^<generator^>
    exit /b 1
)

if "%generator%"=="" (
    echo Usage: %~nx0 ^<config^> ^<generator^>
    exit /b 1
)

@REM ? Define a list of parameter sets
set "parameterSets=build"

@REM ? Delete all directories
for %%D in (%parameterSets%) do (
    echo Deleting directory: %%D
    call scripts\batch\sub_scripts\delete.bat "%%D"
)

@REM ! CONNECTOR

@REM ? Build SDL2_connector
echo Building SDL2_connector...
call scripts\batch\sub_scripts\cmake_build.bat "SDL2_connector\build" "%config%"

@REM ! GAME

@REM ? Configuring build for the game
echo Configuring build for the game...
call scripts\batch\sub_scripts\cmake_config.bat "build" "%config%" "%generator%"

@REM ? Return to the previous directory
cd ..

endlocal
