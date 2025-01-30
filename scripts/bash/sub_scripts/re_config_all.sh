#!/bin/bash

# Set up parameters
jsonFilePath="$1"
config="$2"
generator="$3"

# Check if the required parameters are provided
if [[ -z "$jsonFilePath" || -z "$config" || -z "$generator" ]]; then
    echo "Usage: $0 <jsonFilePath> <config> <generator>"
    exit 1
fi

# Delete all directories
bash ./scripts/bash/delete_all.sh

# Save new build data (using the create_build_data.sh script equivalent in Bash)
echo "Saving new build data to $jsonFilePath"
bash ./scripts/bash/sub_scripts/create_build_data.sh "$jsonFilePath" "$config" "$generator"

# Generator option code for re-using
generatorOption="-G $generator"

# ! CONNECTOR

# Configuring SDL2_connector build
echo "Configuring SDL2_connector build"
bash ./scripts/bash/sub_scripts/cmake_config.sh "SDL2_connector/build" "$config" "$generatorOption"

# Building SDL2_connector
echo "Building SDL2_connector"
bash ./scripts/bash/sub_scripts/cmake_build.sh "SDL2_connector/build" "$config"

# ! GAME

# Configuring game build
echo "Configuring game build"
bash ./scripts/bash/sub_scripts/cmake_config.sh "build" "$config" "$generatorOption"
