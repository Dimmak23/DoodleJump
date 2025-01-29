#!/bin/bash

# Parameters
rebuild=""
config=""
generator=""

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

# Validate conflicting options
if { [[ -n "$generator" || -n "$config" ]] && [[ -n "$rebuild" ]]; }; then
    echo "Wrong command. '--rebuild' can't be used with '--config' and/or '--generator'."
    exit 1
fi

# Define the path to the JSON file
jsonFilePath="build_data.json"

# If NO JSON file exists: create default
if [[ ! -f "$jsonFilePath" ]]; then
    echo "Data about previous build not found. Going with 'Ninja' on 'Debug'..."
	bash ./scripts/bash/sub_scripts/re_config_all.sh "$jsonFilePath" "Debug" "Ninja"
	bash ./scripts/bash/sub_scripts/re_build_game.sh "Debug"
    exit 0
fi

# Read and parse the JSON file
jsonData=$(cat "$jsonFilePath")
currentConfig=$(echo "$jsonData" | jq -r '.Config')
currentGenerator=$(echo "$jsonData" | jq -r '.Generator')

# Validate generator
if [[ -n "$generator" ]]; then
    case "$generator" in
        Ninja)
            echo "Specified $generator as generator."
            ;;
        Makefiles)
            if [[ "$(uname -s)" == "Linux" || "$(uname -s)" == "Darwin" ]]; then
                # generator="Unix Makefiles" #TODO: Currently some problems with such generator, why?
                generator="Ninja"
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
                echo "Using Ninja instread of Visual Studio..."
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
            echo "Invalid generator specified: $generator. Using 'Ninja' as default..."
            generator="Ninja"
            ;;
    esac
else
    generator="$currentGenerator"
    echo "Continue to use $generator as generator..."
fi

# Validate config
if [[ -n "$config" && "$config" != "Debug" && "$config" != "Release" ]]; then
    echo "Couldn't proceed with --config $config option. Using '$currentConfig' as before..."
    config="$currentConfig"
fi

# Handle combinations of parameters
if [[ -n "$config" && -z "$generator" ]]; then
    if [[ "$config" != "$currentConfig" ]]; then
        echo "Configuration changed to: $config."
		bash ./scripts/bash/sub_scripts/re_config_all.sh "$jsonFilePath" "$config" "$currentGenerator"
		bash ./scripts/bash/sub_scripts/re_build_game.sh "$config"
        exit 0
    else
		bash ./scripts/bash/sub_scripts/re_build_game.sh "$currentConfig"
        exit 0
    fi
fi

if [[ -n "$generator" && -z "$config" ]]; then
    if [[ "$generator" != "$currentGenerator" ]]; then
        echo "Generator changed to: $generator. Using 'Debug' as default on new generator..."
		bash ./scripts/bash/sub_scripts/re_config_all.sh "$jsonFilePath" "Debug" "$generator"
		bash ./scripts/bash/sub_scripts/re_build_game.sh "$currentConfig"
        exit 0
    else
		bash ./scripts/bash/sub_scripts/re_build_game.sh "$currentConfig"
        exit 0
    fi
fi

if [[ -n "$config" && -n "$generator" ]]; then
    echo "Redo with config: $config, generator: $generator"
	bash ./scripts/bash/sub_scripts/re_config_all.sh "$jsonFilePath" "$config" "$generator"
	bash ./scripts/bash/sub_scripts/re_build_game.sh "$currentConfig"
    exit 0
fi

if [[ -n "$rebuild" && "$rebuild" != "game" && "$rebuild" != "connector" ]]; then
    echo "Can't rebuild with option --rebuild '$rebuild'. No default option will be used..."
    exit 1
fi

# Handle rebuild cases
if [[ -z "$rebuild" || "$rebuild" == "game" ]]; then
	bash ./scripts/bash/sub_scripts/re_build_game.sh "$currentConfig"
    exit 0
fi

if [[ "$rebuild" == "connector" ]]; then
	bash ./scripts/bash/sub_scripts/re_build_connector.sh "$currentConfig" "$currentGenerator"
	bash ./scripts/bash/sub_scripts/re_build_game.sh "$currentConfig"
    exit 0
fi
