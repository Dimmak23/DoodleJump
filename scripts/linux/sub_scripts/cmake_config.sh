#!/bin/bash

# ? Set up parameters
directoryPath="$1"
config="$2"
generator="$3"

# ? Check if required parameters are provided
if [[ -z "$directoryPath" || -z "$config" ]]; then
    echo "Usage: $0 <directoryPath> <config> [generator]"
    exit 1
fi

# ? Create the directory if it doesn't exist
if [[ ! -d "$directoryPath" ]]; then
    mkdir -p "$directoryPath"
    echo "Directory created: $directoryPath"
else
    echo "Directory already exists: $directoryPath"
fi

# ? Navigate to the directory
cd "$directoryPath" || { echo "Failed to navigate to directory: $directoryPath"; exit 1; }

# ? Generator option code for re-using
generatorOption="-G $generator"

# ? Config option code for re-using
configOption="-DCMAKE_BUILD_TYPE=$config"

# ? Finally config
# TODO: add support for clang compilator
cmake .. -Wno-dev "$configOption" "$generatorOption"
