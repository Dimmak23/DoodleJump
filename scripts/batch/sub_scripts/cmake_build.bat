@echo off
setlocal

@REM ? Check if both parameters are provided
if "%~1"=="" (
    echo Usage: %~nx0 ^<directoryPath^> ^<config^>
    exit /b 1
)

if "%~2"=="" (
    echo Usage: %~nx0 ^<directoryPath^> ^<config^>
    exit /b 1
)

set directoryPath=%1
set config=%2

@REM ? Run the CMake build command
cmake --build "%directoryPath%" --config "%config%" -j 8

endlocal
