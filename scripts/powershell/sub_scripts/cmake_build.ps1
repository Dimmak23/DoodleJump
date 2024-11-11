# Set up parameters
param (
    [string]$directoryPath,
    [string]$config
)

cmake --build $directoryPath --config $config -j 8 -v
