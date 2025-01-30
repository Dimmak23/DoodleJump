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
	& "scripts/powershell/sub_scripts/delete.ps1" -directoryPath $params.directoryPath
}

# Save new build data
& "scripts/powershell/sub_scripts/create_build_data.ps1" -jsonFilePath $jsonFilePath -config $config -generator $generator

# Generator option code for re-using
$generatorOption = "-G $generator"

# #! VENDOR

# # Config
# & "scripts/powershell/sub_scripts/cmake_config.ps1" -directoryPath "vendor/build" -config $config -generatorOption $generatorOption
# Set-Location .. ; Set-Location .. ;
# # Build
# & "scripts/powershell/sub_scripts/cmake_build.ps1" -directoryPath "vendor/build" -config $config

#! CONNECTOR

# Config
& "scripts/powershell/sub_scripts/cmake_config.ps1" -directoryPath "SDL2_connector/build" -config $config -generatorOption $generatorOption
Set-Location .. ; Set-Location .. ;
# Build
& "scripts/powershell/sub_scripts/cmake_build.ps1" -directoryPath "SDL2_connector/build" -config $config

#! GAME

# Config
& "scripts/powershell/sub_scripts/cmake_config.ps1" -directoryPath "build" -config $config -generatorOption $generatorOption
Set-Location .. ;
