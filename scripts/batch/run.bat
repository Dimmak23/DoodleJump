@echo off
setlocal enabledelayedexpansion

@REM ? Ensure a configuration parameter is provided
if "%~1"=="" (
    echo Usage: %~nx0 ^<Debug^|Release^>
    echo Invalid configuration: %config%. Please specify 'Debug' or 'Release'.
    exit /b 1
)

set config=%1

set targetPath=game\source
set exeFileName=

@REM ? Find the first .cpp file in the folder and extract its name
for %%F in ("%targetPath%\*.cpp") do (
    set exeFileName=%%~nF
    goto :found
)

:found
if "%exeFileName%"=="" (
    echo No .cpp file found in the folder.
    exit /b 1
)

@REM ? Run the executable based on the specified configuration
"executables\%config%\%exeFileName%.exe" -window 1600x900
@REM "executables\%config%\%exeFileName%.exe" -window 1200x675
@REM "executables\%config%\%exeFileName%.exe" -fullscreen 1600x900
@REM "executables\%config%\%exeFileName%.exe" -fullscreen 2560x1440

endlocal
