$directoryPath = "build";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "There is no directory: '$directoryPath' exist."
} else {
	Remove-Item .\build -Recurse -Force
};