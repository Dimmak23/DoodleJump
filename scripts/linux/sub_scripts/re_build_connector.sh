#!/bin/bash

# Set up parameters
config="$1"
generator="$2"

# Check if required parameters are provided
if [[ -z "$config" || -z "$generator" ]]; then
    echo "Usage: $0 <config> <generator>"
    exit 1
fi

# Define a list of parameter sets
parameterSets=("build")

# Delete all directories
for directoryPath in "${parameterSets[@]}"; do
    echo "Deleting directory: $directoryPath"
    bash ./scripts/bash/sub_scripts/delete.sh "$directoryPath"
done

# ! CONNECTOR

# Build SDL2_connector
echo "Building SDL2_connector..."
bash ./scripts/bash/sub_scripts/cmake_build.sh "SDL2_connector/build" "$config"

# ! GAME

# Configuring build for the game
echo "Configuring build for the game..."
bash ./scripts/bash/sub_scripts/cmake_config.sh "build" "$config" "$generator"

# Return to the previous directory
cd ..
