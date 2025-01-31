#!/bin/bash

#? Use ./run.sh <config> to run any configured executable

# ? Ensure a configuration parameter is provided
if [[ -z "$1" ]]; then
    echo "Usage: $0 <Debug|Release>"
    echo "Invalid configuration: $config. Please specify 'Debug' or 'Release'."
    exit 1
fi

config="$1"

targetPath="./game/source"

# ? Get the first *.cpp file in the folder
cppFile=$(find "$targetPath" -maxdepth 1 -type f -name "*.cpp" | head -n 1)

if [[ -n "$cppFile" ]]; then
    cppFileName=$(basename "$cppFile")  # ? Extract the file name
    exeFileName="${cppFileName%.cpp}"  # ? Replace .cpp extension with .exe

    # Run the executable based on the specified configuration
    "./executables/$config/$exeFileName" -window 1600x900
    # "./executables/$config/$exeFileName" -window 1200x675
    # "./executables/$config/$exeFileName" -fullscreen 1600x900
    # "./executables/$config/$exeFileName" -fullscreen 2560x1440
else
    echo "No .cpp file found in the folder."
fi
