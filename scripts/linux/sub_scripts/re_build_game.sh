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
bash ./scripts/bash/sub_scripts/delete.sh "$directoryPath"

echo "Using generator: '$generator', and config: '$config'"

if [[ "$generator" == "Unix Makefiles" ]]; then
    directoryPath="executables/$config"

    if [[ ! -d "$directoryPath" ]]; then
        echo "It's Makefiles generator: bring back dummy $directoryPath..."
        mkdir -p "$directoryPath"
    fi
fi

# Build using cmake
echo "Running cmake build for config: $config"
bash ./scripts/bash/sub_scripts/cmake_build.sh "build" "$config"
