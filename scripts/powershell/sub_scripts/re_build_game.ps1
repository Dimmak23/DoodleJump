# Set up parameters
param (
    [string]$config
)

& "scripts/powershell/sub_scripts/delete.ps1" -directoryPath "executables"

# Build
& "scripts/powershell/sub_scripts/cmake_build.ps1" -directoryPath "build" -config $config
