@echo off
setlocal

@REM ? Set up parameters
set "directoryPath=%~1"

@REM ? Check if directoryPath is provided
if "%directoryPath%"=="" (
    echo Usage: %~nx0 ^<directoryPath^>
    exit /b 1
)

@REM ? Check if directory exists
if not exist "%directoryPath%" (
    echo There is no directory: '%directoryPath%' exists.
    exit /b 0
)

@REM ? Delete the directory
echo Deleting directory: %directoryPath%
rmdir /s /q "%directoryPath%"

endlocal
