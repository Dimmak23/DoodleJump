#!/bin/bash

# List of folder destinations
folderList=(
    "executables"
    "build"
    "SDL2_connector/build"
    "SDL2_connector/bin"
    "vendor/build"
    "vendor/SDL2"
    "vendor/ZLIB"
    "vendor/JPEG"
    "vendor/PNG"
    "vendor/SDL2_image"
)

# Path to the sub-script file
subScriptPath="scripts/bash/sub_scripts/delete.sh"

# Check if the sub-script exists
if [[ ! -f "$subScriptPath" ]]; then
    echo "Sub-script not found: $subScriptPath"
    exit 1
fi

# Run the sub-script for each folder
for folder in "${folderList[@]}"; do
    echo "Processing folder: $folder"
    bash "$subScriptPath" "$folder"
done
