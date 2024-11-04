# Set up parameters
param (
    [string]$config
)

$directoryPath = "executables";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
	Write-Host "There is no directory: '$directoryPath' exist."
} else {
	Remove-Item $directoryPath -Recurse -Force
};

# Build
& "sub_scripts/cmake_build.ps1" -directoryPath "build" -config $config
