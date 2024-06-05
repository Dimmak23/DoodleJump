$directoryPath = "build";

if (-not (Test-Path -Path $directoryPath -PathType Container)) {
    New-Item -ItemType Directory -Path $directoryPath | Out-Null
    Write-Host "Directory created: $directoryPath"
} else {
    Write-Host "Directory already exists: $directoryPath"
};

Set-Location build ; cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Release -G Ninja ; Set-Location ..
# Set-Location build ; cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ; Set-Location ..
# Set-Location build ; cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" ; Set-Location ..