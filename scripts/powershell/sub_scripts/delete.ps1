# Set up parameters
param (
    [string]$directoryPath
)

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
	Write-Host "There is no directory: '$directoryPath' exist."
} else {
	Remove-Item $directoryPath -Recurse -Force
};
