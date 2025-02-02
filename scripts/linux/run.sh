#!/bin/bash

set -e  # Exit on error

# TODO: Maybe passing configuration type will be used in the future 
# #? Use ./run.sh <config> to run any configured executable
# # ? Ensure a configuration parameter is provided
# if [[ -z "$1" ]]; then
#     echo "Usage: $0 <Debug|Release>"
#     echo "Invalid configuration: $config. Please specify 'Debug' or 'Release'."
#     exit 1
# fi

# config="$1"
# TODO: Maybe passing configuration type will be used in the future 

targetPath="game/source"
exeFileName=""
fullFilePath=""

# ? Find the first .cpp file in the folder and extract its name
for file in "$targetPath"/*.cpp; do
    if [ -f "$file" ]; then
        exeFileName="$(basename "$file" .cpp)"
        break
    fi
done

if [ -z "$exeFileName" ]; then
    echo "No .cpp file found in the folder."
    exit 1
fi

# ? Define the base folder where executables might be located
BASE_FOLDER="$(pwd)/executables"

# ? Check for Debug or Release folder
if [ -d "$BASE_FOLDER/Release" ]; then
    # ? Check if the executable exists
    if [ -f "$BASE_FOLDER/Release/$exeFileName" ]; then
        fullFilePath="$BASE_FOLDER/Release/$exeFileName"
    fi
fi

if [ -d "$BASE_FOLDER/Debug" ]; then
    # ? Check if the executable exists
    if [ -f "$BASE_FOLDER/Debug/$exeFileName" ]; then
        fullFilePath="$BASE_FOLDER/Debug/$exeFileName"
    fi
fi

if [ ! -f "$BASE_FOLDER/Release/$exeFileName" ]; then
    if [ ! -f "$BASE_FOLDER/Debug/$exeFileName" ]; then
        echo "No $BASE_FOLDER/Release/$exeFileName or $BASE_FOLDER/Debug/$exeFileName file have been found!"
        exit 1
    fi
fi

# "$fullFilePath" -window 1600x900
"$fullFilePath" -window 1200x675
# "$fullFilePath" -fullscreen 1600x900
# "$fullFilePath" -fullscreen 2560x1440

# TODO: Maybe passing configuration type will be used in the future 
    # Run the executable based on the specified configuration
    # "./executables/$config/$exeFileName" -window 1600x900
    # "./executables/$config/$exeFileName" -window 1200x675
    # "./executables/$config/$exeFileName" -fullscreen 1600x900
    # "./executables/$config/$exeFileName" -fullscreen 2560x1440
# TODO: Maybe passing configuration type will be used in the future 
