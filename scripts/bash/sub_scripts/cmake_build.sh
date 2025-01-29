#!/bin/bash

# Set up parameters
directoryPath="$1"
config="$2"

# Check if both parameters are provided
if [[ -z "$directoryPath" || -z "$config" ]]; then
    echo "Usage: $0 <directoryPath> <config>"
    exit 1
fi

# Run the cmake build command
cmake --build "$directoryPath" --config "$config" -j 8 -v
