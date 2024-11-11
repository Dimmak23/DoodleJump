# Set up parameters
param (
    [string]$directoryPath,
    [string]$config,
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
# Build type code for re-using
if($config -eq "Debug"){
	cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Debug $generatorOption
}
else{
	cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Release $generatorOption
}
