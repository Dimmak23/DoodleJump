# Set up parameters
param (
    [string]$config,
    [string]$generator
)

$directoryPath = "executables"
& "scripts/powershell/sub_scripts/delete.ps1" -directoryPath $directoryPath

if ($generator -eq "MinGW Makefiles") {
    if (-not(Test-Path $directoryPath)) {
        # TODO: Check, maybe we can delete this
        $directoryPath = "executables/$config"
        Write-Host "It's Makefiles generator: bring back dummy $directoryPath..."
        New-Item -Path $directoryPath -ItemType Directory
    }
} 

& "scripts/powershell/sub_scripts/cmake_build.ps1" -directoryPath "build" -config $config
