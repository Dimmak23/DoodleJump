#!/bin/bash

# ? Parameters
rebuild=""
config=""
generator=""

# ^ Validations

while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        --rebuild)
            rebuild="$2"
            shift
            shift
            ;;
        --config)
            config="$2"
            shift
            shift
            ;;
        --generator)
            generator="$2"
            shift
            shift
            ;;
        *)
            echo "Invalid parameter: $1"
            exit 1
            ;;
    esac
done

# ? Validate conflicting options
if { [[ -n "$generator" || -n "$config" ]] && [[ -n "$rebuild" ]]; }; then
    echo "Wrong command. '--rebuild' can't be used with '--config' and/or '--generator'."
    exit 1
fi

# ? Does 'rebuild' option is valid?
if [[ -n "$rebuild" && "$rebuild" != "game" && "$rebuild" != "connector" ]]; then
    echo "Can't rebuild with option --rebuild '$rebuild'. No default option will be used..."
    exit 1
fi

# ? Validate generator
if [[ -n "$generator" ]]; then
    case "$generator" in
        Ninja)
            echo "Specified $generator as generator."
            ;;
        Makefiles)
            if [[ "$(uname -s)" == "Linux" || "$(uname -s)" == "Darwin" ]]; then
                generator="Unix Makefiles"
            elif [[ "$(uname -s)" == "MINGW"* || "$(uname -s)" == "MSYS"* || "$(uname -s)" == "CYGWIN"* ]]; then
                generator="MinGW Makefiles"
            else
                echo "Unknown system, using default generator."
                generator="Ninja"
            fi
            echo "Specified $generator as generator."
            ;;
        VS)
            if [[ "$(uname -s)" == "Linux" || "$(uname -s)" == "Darwin" ]]; then
                echo "Using Ninja instead of Visual Studio..."
                generator="Ninja"
            elif [[ "$(uname -s)" == "MINGW"* || "$(uname -s)" == "MSYS"* || "$(uname -s)" == "CYGWIN"* ]]; then
                generator="Visual Studio 17 2022"
            else
                echo "Unknown system, using default generator."
                generator="Ninja"
            fi
            echo "Specified $generator as generator."
            ;;
        *)
            echo "Invalid generator specified: $generator. Checking previous built data, if have any..."
            ;;
    esac
else
    echo "No generator specified! Checking previous built data, if have any..."
fi

# ? Validate config
if [[ "$config" != "Debug" && "$config" != "Release" ]]; then
    echo "No or corrupt config have been provided. Using previous config instead if we have any..."
fi

# ? Define the path to the JSON file
jsonFilePath="build_data.json"

echo "Checking built data..."
# ? If NO JSON file exists: create default
if [[ ! -f "$jsonFilePath" ]]; then
    echo "Data about previous build not found. 'Debug' and 'Ninja' will be written as built data."
    bash ./scripts/linux/sub_scripts/create_build_data.sh "$jsonFilePath" "Debug" "Ninja"
else
    echo "Got built data file..."
fi

# ? In any case we have built data now
jsonData=$(cat "$jsonFilePath")
currentConfig=$(echo "$jsonData" | jq -r '.Config')
currentGenerator=$(echo "$jsonData" | jq -r '.Generator')

# ^ Handle combinations of parameters

# * build.sh
# ? Doing this only when 'config' and 'generator' parameters didn't provided
if [[ -z "$rebuild" && -z "$config" && -z "$generator" ]]; then
    echo "command were: 'bash.sh'. Going with data about previous built: --config $currentConfig --generator $currentGenerator"
    bash ./scripts/linux/sub_scripts/re_config_all.sh "$jsonFilePath" "$currentConfig" "$currentGenerator"
    bash ./scripts/linux/sub_scripts/re_build_game.sh "$currentConfig" "$currentGenerator"
    exit 0
fi

# * build.sh --config 'Debug'/'Release'
# ? Check if new configuration: re-configure all with same generator
if [[ -n "$config" && -z "$generator" ]]; then
    if [[ "$config" != "$currentConfig" ]]; then
        echo "command were: 'bash.sh --config $config'"
        echo "Configuration changed to: $config."
		bash ./scripts/linux/sub_scripts/re_config_all.sh "$jsonFilePath" "$config" "$currentGenerator"
		bash ./scripts/linux/sub_scripts/re_build_game.sh "$config" "$currentGenerator"
        exit 0
    else
		bash ./scripts/linux/sub_scripts/re_build_game.sh "$config" "$currentGenerator"
        exit 0
    fi
fi

# * build.sh --generator 'Ninja'/'Makefiles'/'VS'
# ? Check if new generator: re-configure all with same configuration
if [[ -n "$generator" && -z "$config" ]]; then
    if [[ "$generator" != "$currentGenerator" ]]; then
        echo "command were: 'bash.sh --generator $generator'"
		bash ./scripts/linux/sub_scripts/re_config_all.sh "$jsonFilePath" "$currentConfig" "$generator"
		bash ./scripts/linux/sub_scripts/re_build_game.sh "$currentConfig" "$generator"
        exit 0
    else
		bash ./scripts/linux/sub_scripts/re_build_game.sh "$currentConfig" "$generator"
        exit 0
    fi
fi

# * build.sh --generator 'Ninja'/'MinGW'/'VS' --config 'Debug'/'Release'
# * build.sh --config 'Debug'/'Release' --generator 'Ninja'/'MinGW'/'VS'
# ? Could be same as 'build.sh'
if [[ -n "$config" && -n "$generator" ]]; then
    echo "Redo with config: $config, generator: $generator"
	bash ./scripts/linux/sub_scripts/re_config_all.sh "$jsonFilePath" "$config" "$generator"
	bash ./scripts/linux/sub_scripts/re_build_game.sh "$config" "$generator"
    exit 0
fi

# * build.sh --rebuild game
# ? Rebuilding only game: only game source code have been changed
if [[ "$rebuild" == "game" ]]; then
    echo "Just rebuilding game code space..."
	bash ./scripts/linux/sub_scripts/re_build_game.sh "$currentConfig" "$currentGenerator"
    exit 0
fi

# * build.sh --rebuild connector
# ? Rebuilding connector and game: connector and game source codes have been changed
if [[ "$rebuild" == "connector" ]]; then
    echo "Just rebuilding connector and game code space..."
	bash ./scripts/linux/sub_scripts/re_build_connector.sh "$currentConfig" "$currentGenerator"
	bash ./scripts/linux/sub_scripts/re_build_game.sh "$currentConfig" "$currentGenerator"
    exit 0
fi
