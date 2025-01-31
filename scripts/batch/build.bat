@echo off
setlocal enabledelayedexpansion

@REM ? Parameters
set "rebuild="
set "config="
set "generator="

@REM ! Validations

:parse_args
if "%~1"=="" goto validate_options
if "%~1"=="--rebuild" (
    set "rebuild=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="--config" (
    set "config=%~2"
    shift
    shift
    goto parse_args
)
if "%~1"=="--generator" (
    set "generator=%~2"
    shift
    shift
    goto parse_args
)
echo Invalid parameter: %1
exit /b 1

@REM ? Validate conflicting options
:validate_options
if not "!generator!!config!"=="" if not "!rebuild!"=="" (
    echo Wrong command. "--rebuild" can't be used with "--config" and/or "--generator".
    exit /b 1
)

@REM ? Does 'rebuild' option is valid?
if not "!rebuild!"=="" if not "!rebuild!"=="game" if not "!rebuild!"=="connector" (
    echo Can't rebuild with option --rebuild '!rebuild!'. No default option will be used...
    exit /b 1
)

@REM ? Validate generator
if not "!generator!"=="" (
    if "!generator!"=="Ninja" (
        echo Specified !generator! as generator.
    ) else if "!generator!"=="Makefiles" (
        set "generator=MinGW Makefiles"
		echo Specified !generator! as generator.
    ) else if "!generator!"=="VS" (
		@REM TODO: What if system doesn't have this version, but has some earliar?
        set "generator=Visual Studio 17 2022"
		echo Specified !generator! as generator.
    ) else (
        echo Invalid generator specified: !generator!. Checking previous built data, if any...
    )
) else (
    echo No generator specified! Checking previous built data, if any...
)

@REM ? Validate config
if not "!config!"=="Debug" if not "!config!"=="Release" (
    echo No or corrupt config provided. Using config from built data if available...
)

@REM ? Define JSON file path
set "jsonFilePath=build_data.json"

echo Checking built data...
@REM ? Check if JSON file exists
if not exist "!jsonFilePath!" (
    echo Data about previous build not found. 'Debug' and 'Ninja' will be written as built data.
    call scripts\batch\sub_scripts\create_build_data.bat "!jsonFilePath!" "Debug" "Ninja"
) else (
	echo Got built data file...
)

@REM ? Read JSON data

@REM ? Read the JSON file and extract values
for /f "tokens=2 delims=:," %%a in ('findstr /i "Config" "!jsonFilePath!"') do set "currentConfig=%%~a"
for /f "tokens=2 delims=:," %%a in ('findstr /i "Generator" "!jsonFilePath!"') do set "currentGenerator=%%~a"

@REM ? Remove leading and trailing spaces from the extracted values
set "currentConfig=!currentConfig:~2,-1!"
set "currentGenerator=!currentGenerator:~2,-1!"

@REM ? Output the result
echo Current Config: !currentConfig!
echo Current Generator: !currentGenerator!

@REM ! Handle parameter combinations

@REM * build.bat
@REM ? Doing this only when 'config' and 'generator' parameters didn't provided
if "!rebuild!!config!!generator!"=="" (
    echo command was: 'build.bat'. Using previous build data: --config !currentConfig! --generator !currentGenerator!
    call scripts\batch\sub_scripts\re_config_all.bat "!jsonFilePath!" "!currentConfig!" "!currentGenerator!"
    call scripts\batch\sub_scripts\re_build_game.bat "!currentConfig!" "!currentGenerator!"
    exit /b 0
)

@REM * build.bat --config 'Debug'/'Release'
@REM ? Check if new configuration: re-configure all with same generator
if not "!config!"=="" if "!generator!"=="" (
    if not "!config!"=="!currentConfig!" (
        echo command was: 'build.bat --config !config!'
        call scripts\batch\sub_scripts\re_config_all.bat "!jsonFilePath!" "!config!" "!currentGenerator!"
    )
    call scripts\batch\sub_scripts\re_build_game.bat "!config!" "!currentGenerator!"
    exit /b 0
)

@REM * build.bat --generator 'Ninja'/'Makefiles'/'VS'
@REM ? Check if new generator: re-configure all with same configuration
if not "!generator!"=="" if "!config!"=="" (
    if not "!generator!"=="!currentGenerator!" (
        echo command was: 'build.bat --generator !generator!'
        call scripts\batch\sub_scripts\re_config_all.bat "!jsonFilePath!" "!currentConfig!" "!generator!"
    )
    call scripts\batch\sub_scripts\re_build_game.bat "!currentConfig!" "!generator!"
    exit /b 0
)

@REM * build.bat --generator 'Ninja'/'MinGW'/'VS' --config 'Debug'/'Release'
@REM * build.bat --config 'Debug'/'Release' --generator 'Ninja'/'MinGW'/'VS'
@REM ? Could be same as 'build.bat'
if not "!config!"=="" if not "!generator!"=="" (
    echo Redo with config: !config!, generator: !generator!
    call scripts\batch\sub_scripts\re_config_all.bat "!jsonFilePath!" "!config!" "!generator!"
    call scripts\batch\sub_scripts\re_build_game.bat "!config!" "!generator!"
    exit /b 0
)

@REM * build.bat --rebuild game
@REM ? Rebuilding only game: only game source code have been changed
if "!rebuild!"=="game" (
    echo Just rebuilding game code space...
    call scripts\batch\sub_scripts\re_build_game.bat "!currentConfig!" "!currentGenerator!"
    exit /b 0
)

@REM * build.bat --rebuild connector
@REM ? Rebuilding connector and game: connector and game source codes have been changed
if "!rebuild!"=="connector" (
    echo Just rebuilding connector and game code space...
    call scripts\batch\sub_scripts\re_build_connector.bat "!currentConfig!" "!currentGenerator!"
    call scripts\batch\sub_scripts\re_build_game.bat "!currentConfig!" "!currentGenerator!"
    exit /b 0
)

exit /b 0
