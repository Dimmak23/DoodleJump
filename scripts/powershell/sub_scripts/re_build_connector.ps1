# Set up parameters
param (
    [string]$config,
    [string]$generator
)

# Define a list of parameter sets
$parameterSets = @(
	@{ directoryPath = "build"},
	@{ directoryPath = "SDL2_connector/build"},
	@{ directoryPath = "SDL2_connector/bin"}
)

# Delete all
foreach ($params in $parameterSets) {
	& "delete.ps1" -directoryPath $params.directoryPath
}

# Build type code for re-using
$buildType = "-DCMAKE_BUILD_TYPE=`"$config`""

# Generator option code for re-using
$generatorOption = "-G $generator"

#! CONNECTOR

# Config
& "cmake_config.ps1" -directoryPath "SDL2_connector/build" -buildType $buildType -generatorOption $generatorOption
Set-Location .. ; Set-Location .. ;
# Build
& "cmake_build.ps1" -directoryPath "SDL2_connector/build" -config $config

#! GAME

# Config
& "cmake_config.ps1" -directoryPath "build" -buildType $buildType -generatorOption $generatorOption
Set-Location .. ;
