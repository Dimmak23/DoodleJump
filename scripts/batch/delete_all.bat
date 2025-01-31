@echo off
setlocal enabledelayedexpansion

@REM ? List of folder destinations
set folders=executables build SDL2_connector\build SDL2_connector\bin

@REM ? Path to the sub-script file
set subScriptPath=scripts\batch\sub_scripts\delete.bat

@REM ? Check if the sub-script exists
if not exist "%subScriptPath%" (
    echo Sub-script not found: %subScriptPath%
    exit /b 1
)

@REM ? Run the sub-script for each folder
for %%F in (%folders%) do (
    echo Processing folder: %%F
    call "%subScriptPath%" "%%F"
)

endlocal
