# Set up parameters
param (
    [string]$directoryPath,
    [string]$buildType,
    [string]$generatorOption
)

# Config
if (-not (Test-Path -Path $directoryPath -PathType Container)) {
	New-Item -ItemType Directory -Path $directoryPath | Out-Null
	Write-Host "Directory created: $directoryPath"
} else {
	Write-Host "Directory already exists: $directoryPath"
};
Set-Location $directoryPath ;
# Write-Output "Current location: $(Get-Location)" ;
cmake .. -Wno-dev $buildType $generatorOption
