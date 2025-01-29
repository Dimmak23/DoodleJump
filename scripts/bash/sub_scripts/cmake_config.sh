#!/bin/bash

# Set up parameters
directoryPath="$1"
config="$2"
generatorOption="$3"


# Check if required parameters are provided
if [[ -z "$directoryPath" || -z "$config" ]]; then
    echo "Usage: $0 <directoryPath> <config> [generatorOption]"
    exit 1
fi

# Create the directory if it doesn't exist
if [[ ! -d "$directoryPath" ]]; then
    mkdir -p "$directoryPath"
    echo "Directory created: $directoryPath"
else
    echo "Directory already exists: $directoryPath"
fi

# Navigate to the directory
cd "$directoryPath" || { echo "Failed to navigate to directory: $directoryPath"; exit 1; }

echo "! Using as generator: [$generatorOption]..."

# Build type configuration and run cmake
if [[ "$config" == "Debug" ]]; then
    if [[ "$generatorOption" == "-G Unix Makefiles" ]]; then
        cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"
    else
        cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Debug $generatorOption
    fi
else
    if [[ "$generatorOption" == "-G Unix Makefiles" ]]; then
        cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
    else
        cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Release $generatorOption
    fi
fi
