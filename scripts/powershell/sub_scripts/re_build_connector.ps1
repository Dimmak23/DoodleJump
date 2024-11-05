# Set up parameters
param (
    [string]$config,
    [string]$generator
)

# Define a list of parameter sets
$parameterSets = @(
	@{ directoryPath = "build"}
)

# Delete all
foreach ($params in $parameterSets) {
	& "scripts/powershell/sub_scripts/delete.ps1" -directoryPath $params.directoryPath
}

# Generator option code for re-using
$generatorOption = "-G $generator"

#! CONNECTOR

# Build
& "scripts/powershell/sub_scripts/cmake_build.ps1" -directoryPath "SDL2_connector/build" -config $config

#! GAME

# Config
& "scripts/powershell/sub_scripts/cmake_config.ps1" -directoryPath "build" -config $config -generatorOption $generatorOption
Set-Location .. ;
