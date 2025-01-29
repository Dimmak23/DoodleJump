#!/bin/bash

# Set up parameters
config="$1"

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

# Set the directory path based on the config
directoryPath="executables/$config"

# Build using cmake
echo "Running cmake build for config: $config"
bash ./scripts/bash/sub_scripts/cmake_build.sh "build" "$config"
