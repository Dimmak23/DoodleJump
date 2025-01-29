#!/bin/bash

# Set up parameters
directoryPath="$1"

if [[ -z "$directoryPath" ]]; then
    echo "Usage: $0 <directoryPath>"
    exit 1
fi

if [[ ! -d "$directoryPath" ]]; then
    echo "There is no directory: '$directoryPath' exists."
else
    echo "Deleting directory: $directoryPath"
    rm -rf "$directoryPath"
fi
