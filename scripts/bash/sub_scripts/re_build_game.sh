#!/bin/bash

# Set up parameters
config="$1"
generator=$2

# Check if the required parameter is provided
if [[ -z "$config" ]]; then
    echo "Usage: $0 <config>"
    exit 1
fi

# Define the directory paths
directoryPath="executables"

# Delete the 'executables' directory
echo "Deleting directory: $directoryPath"
bash ./scripts/bash/sub_scripts/delete.sh "$directoryPath"

# TODO: Check, maybe we can delete this
# Set the directory path based on the config
# directoryPath="executables/$config"

if [[ "$generator" == "Unix Makefiles" ]]; then
    directoryPath="executables/$config"

    if [[ ! -d "$directoryPath" ]]; then
        echo "It's Makefiles generator: bring back dummy $directoryPath..."
        mkdir "$directoryPath"
    fi
fi

# Build using cmake
echo "Running cmake build for config: $config"
bash ./scripts/bash/sub_scripts/cmake_build.sh "build" "$config"
