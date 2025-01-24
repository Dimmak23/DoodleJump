# List of folder destinations
$folderList = @(
    "executables",
    "build",
    "SDL2_connector/build",
    "SDL2_connector/bin",
    "vendor/build",
    "vendor/SDL2",
    "vendor/ZLIB",
    "vendor/JPEG",
    "vendor/PNG",
    "vendor/SDL2_image"
)

# Path to the sub-script file
$subScriptPath = "scripts/powershell/sub_scripts/delete.ps1"

# Run the sub-script for each folder
foreach ($folder in $folderList) {
    Write-Host "Processing folder: $folder"
    PowerShell -File $subScriptPath -directoryPath $folder
}
