@echo off
setlocal enabledelayedexpansion

@REM TODO: Maybe passing configuration type will be used in the future 
@REM @REM ? Ensure a configuration parameter is provided
@REM if "%~1"=="" (
@REM     echo Usage: %~nx0 ^<Debug^|Release^>
@REM     echo Invalid configuration: %config%. Please specify 'Debug' or 'Release'.
@REM     exit /b 1
@REM )

@REM set config=%1
@REM TODO: Maybe passing configuration type will be used in the future 

set "targetPath=game\source"
set "exeFileName="
set "fullFilePath="

@REM ? Find the first .cpp file in the folder and extract its name
for %%F in ("%targetPath%\*.cpp") do (
    set "exeFileName=%%~nF"
    goto :found
)

:found
if "%exeFileName%"=="" (
    echo No .cpp file found in the folder.
    exit /b 1
)

@REM ? Define the base folder where executables might be located
set "BASE_FOLDER=%CD%\executables"

@REM ? Check for Release or Debug folder
@REM ? Later for executable existance
if exist "%BASE_FOLDER%\Release" (
    echo "%BASE_FOLDER%\Release" folder found!
    if exist "%BASE_FOLDER%\Release\%exeFileName%.exe" (
        set "fullFilePath=%BASE_FOLDER%\Release\%exeFileName%.exe"
    )
)
if exist "%BASE_FOLDER%\Debug" (
    echo "%BASE_FOLDER%\Debug" folder found!
    if exist "%BASE_FOLDER%\Debug\%exeFileName%.exe" (
        set "fullFilePath=%BASE_FOLDER%\Debug\%exeFileName%.exe"
    )
)
if not exist "%BASE_FOLDER%\Release\%exeFileName%.exe" (
    if not exist "%BASE_FOLDER%\Debug\%exeFileName%.exe" (
        echo No executables/Debug/%exeFileName%.exe or executables/Release/%exeFileName%.exe file found!
        exit /b 1
    )
)

if "%fullFilePath%"=="" (
    echo Executable file not found.
    exit /b 1
)

@REM ? Run the executable
@REM "%fullFilePath%" -window 1600x900
@REM "%fullFilePath%" -window 1200x675
@REM "%fullFilePath%" -fullscreen 1600x900
"%fullFilePath%" -fullscreen 2560x1440

@REM TODO: Maybe passing configuration type will be used in the future 
@REM @REM ? Run the executable based on the specified configuration
@REM @REM "executables\%config%\%exeFileName%.exe" -window 1600x900
@REM @REM "executables\%config%\%exeFileName%.exe" -window 1200x675
@REM "executables\%config%\%exeFileName%.exe" -fullscreen 1600x900
@REM @REM "executables\%config%\%exeFileName%.exe" -fullscreen 2560x1440
@REM TODO: Maybe passing configuration type will be used in the future 

endlocal