#!/bin/bash

# Set up parameters
jsonFilePath="$1"
config="$2"
generator="$3"

# Check if required parameters are provided
if [[ -z "$jsonFilePath" || -z "$config" || -z "$generator" ]]; then
    echo "Usage: $0 <jsonFilePath> <config> <generator>"
    exit 1
fi

# Create a JSON string with the parameters
jsonData=$(cat <<EOF
{
    "Config": "$config",
    "Generator": "$generator"
}
EOF
)

# Write the JSON data to a file
echo "$jsonData" > "$jsonFilePath"

echo "JSON data written to $jsonFilePath"
