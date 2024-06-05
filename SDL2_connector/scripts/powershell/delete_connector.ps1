$directoryPath = "bin";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};
