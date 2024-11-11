# Set up parameters
param (
    [string]$config
)

$directoryPath = "executables"
& "scripts/powershell/sub_scripts/delete.ps1" -directoryPath $directoryPath
$directoryPath = "executables/$config"

# if (-not (Test-Path -Path $directoryPath -PathType Container)) {
#     New-Item -ItemType Directory -Path $directoryPath | Out-Null
#     Write-Host "Directory created: $directoryPath"
# } else {
#     Write-Host "Directory already exists: $directoryPath"
# };

& "scripts/powershell/sub_scripts/cmake_build.ps1" -directoryPath "build" -config $config
