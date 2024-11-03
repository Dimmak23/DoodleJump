# Set up parameters
param (
    [string]$config,
    [string]$rebuild,
    [string]$generator
)

# Define the path to the JSON file
$jsonFilePath = "build_data.json"

# If NO JSON file exists: create default
if (-not (Test-Path $jsonFilePath)) {
    & "sub_scripts/create_build_data.ps1" -jsonFilePath $jsonFilePath -config "Debug" -generator "Ninja"
}

# Check if the JSON file exists
if (Test-Path $jsonFilePath) {
    # Read and parse the JSON file
    $jsonData = Get-Content -Path $jsonFilePath | ConvertFrom-Json

    # Check if the current parameters match the JSON file values
    $isMatch = ($config -eq $jsonData.Config) -and ($generator -eq $jsonData.Generator)

    if (-not $isMatch) {
        # Parameters differ from JSON file values
        Write-Output "Parameters differ. Performing action for differing parameters."

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
            & "sub_scripts/delete.ps1" -directoryPath $params.directoryPath
        }
        
        # Save new build data
        & "sub_scripts/create_build_data.ps1" -jsonFilePath $jsonFilePath -config $config -generator $generator

        # Build type code for re-using
        $buildType = "-DCMAKE_BUILD_TYPE=`"$config`""
        
        # Generator option code for re-using
        $generatorOption = "-G $generator"

        #! VENDOR

        # Config
        & "sub_scripts/cmake_config.ps1" -directoryPath "vendor/build" -buildType $buildType -generatorOption $generatorOption
        Set-Location .. ; Set-Location .. ;
        # Build
        & "sub_scripts/cmake_build.ps1" -directoryPath "vendor/build" -config $config

        #! CONNECTOR

        # Config
        & "sub_scripts/cmake_config.ps1" -directoryPath "SDL2_connector/build" -buildType $buildType -generatorOption $generatorOption
        Set-Location .. ; Set-Location .. ;
        # Build
        & "sub_scripts/cmake_build.ps1" -directoryPath "SDL2_connector/build" -config $config

        #! GAME

        # Config
        & "sub_scripts/cmake_config.ps1" -directoryPath "build" -buildType $buildType -generatorOption $generatorOption
        Set-Location .. ;
        # Build
        # ? Now build step here... just wait for it

    }

    $directoryPath = "executables";

    if (-not (Test-Path -Path $directoryPath -PathType Container)) {
        Write-Host "There is no directory: '$directoryPath' exist."
    } else {
        Remove-Item $directoryPath -Recurse -Force
    };

    # Build
    & "sub_scripts/cmake_build.ps1" -directoryPath "build" -config $config
}
