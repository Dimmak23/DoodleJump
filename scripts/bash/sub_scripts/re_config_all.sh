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

# Define a list of parameter sets
parameterSets=("build" "SDL2_connector/build" "SDL2_connector/bin" "vendor/build" "vendor/SDL2" "vendor/SDL2_image" "vendor/JPEG" "vendor/PNG")

# Delete all directories
for directoryPath in "${parameterSets[@]}"; do
    echo "Deleting directory: $directoryPath"
    bash ./scripts/bash/sub_scripts/delete.sh "$directoryPath"
done

# Save new build data (using the create_build_data.sh script equivalent in Bash)
echo "Saving new build data to $jsonFilePath"
bash ./scripts/bash/sub_scripts/create_build_data.sh "$jsonFilePath" "$config" "$generator"

# Generator option code for re-using
generatorOption="-G $generator"

# # ! VENDOR

# # Configuring vendor build
# echo "Configuring vendor build"
# bash ./scripts/bash/sub_scripts/cmake_config.sh "vendor/build" "$config" "$generatorOption"

# echo "current folder: $(pwd)"

# # Building vendor
# echo "Building vendor"
# bash ./scripts/bash/sub_scripts/cmake_build.sh "vendor/build" "$config"

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
