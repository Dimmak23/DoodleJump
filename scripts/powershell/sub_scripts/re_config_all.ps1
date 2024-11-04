# Set up parameters
param (
    [string]$jsonFilePath,
    [string]$config,
    [string]$generator
)

# Define a list of parameter sets
$parameterSets = @(
	@{ directoryPath = "build"},
	@{ directoryPath = "SDL2_connector/build"},
	@{ directoryPath = "SDL2_connector/bin"},
	@{ directoryPath = "vendor/build"},
	@{ directoryPath = "vendor/SDL2"},
	@{ directoryPath = "vendor/SDL2_image"},
	@{ directoryPath = "vendor/JPEG"},
	@{ directoryPath = "vendor/PNG"}
)

# Delete all
foreach ($params in $parameterSets) {
	& "delete.ps1" -directoryPath $params.directoryPath
}

# Save new build data
& "create_build_data.ps1" -jsonFilePath $jsonFilePath -config $config -generator $generator

# Build type code for re-using
$buildType = "-DCMAKE_BUILD_TYPE=`"$config`""

# Generator option code for re-using
$generatorOption = "-G $generator"

#! VENDOR

# Config
& "cmake_config.ps1" -directoryPath "vendor/build" -buildType $buildType -generatorOption $generatorOption
Set-Location .. ; Set-Location .. ;
# Build
& "cmake_build.ps1" -directoryPath "vendor/build" -config $config

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
