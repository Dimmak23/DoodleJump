# Set up parameters
param (
    [string]$config
)

$directoryPath = "executables"
& "scripts/powershell/sub_scripts/delete.ps1" -directoryPath $directoryPath
$directoryPath = "executables/$config"

& "scripts/powershell/sub_scripts/cmake_build.ps1" -directoryPath "build" -config $config
