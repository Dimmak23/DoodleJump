$directoryPath = "executables";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "There is no directory: '$directoryPath' exist."
} else {
	Remove-Item $directoryPath -Recurse -Force
};

$directoryPath = "build";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "There is no directory: '$directoryPath' exist."
} else {
	Remove-Item .\build -Recurse -Force
};

$stampFile = ".\build_info.txt";

if (-not (Test-Path -Path $stampFile)) {
    Write-Host "Time stamp file: '$stampFile' doesn't exist."
} else {
	Remove-Item -Path $stampFile -Force
};

$directoryPath = "SDL2_connector/build";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "There is no directory: '$directoryPath' exist."
} else {
	Remove-Item $directoryPath -Recurse -Force
};

$directoryPath = "SDL2_connector/bin";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$stampFile = "SDL2_connector/build_info.txt";

if (-not (Test-Path -Path $stampFile)) {
    Write-Host "Time stamp file: '$stampFile' doesn't exist."
} else {
	Remove-Item -Path $stampFile -Force
};

$directoryPath = "vendor/build";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "There is no directory: '$directoryPath' exist."
} else {
	Remove-Item $directoryPath -Recurse -Force
};

$directoryPath = "vendor/SDL2";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$directoryPath = "vendor/SDL2_image";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$directoryPath = "vendor/JPEG";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$directoryPath = "vendor/PNG";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    Write-Host "Directory: '$directoryPath' doesn't exist."
} else {
	Remove-Item .\$directoryPath -Recurse -Force
};

$stampFile = "vendor/build_info.txt";

if (-not (Test-Path -Path $stampFile)) {
    Write-Host "Time stamp file: '$stampFile' doesn't exist."
} else {
	Remove-Item -Path $stampFile -Force
};


