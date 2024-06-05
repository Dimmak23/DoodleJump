$directoryPath = "SDL2";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$directoryPath = "SDL2_image";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$directoryPath = "JPEG";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$directoryPath = "PNG";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$stampFile = ".\build_info.txt";

if (-not (Test-Path -Path $stampFile)) {
    Write-Host "Time stamp file: '$stampFile' doesn't exist."
} else {
	Remove-Item -Path $stampFile -Force
};
