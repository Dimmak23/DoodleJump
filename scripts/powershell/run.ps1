param (
    [string]$config
)

if (-not $config) {
    Write-Host "Usage: .\script.ps1 -config <Debug|Release>"
    exit 1
}

if ($config -ne "Debug" -and $config -ne "Release") {
    Write-Host "Invalid configuration: $config. Please specify 'Debug' or 'Release'."
    exit 1
}

$targetPath = ".\game\source"
$exeFolderPath = ".\executables\$config"

# Check if the configuration folder exists
if (-not (Test-Path -Path $exeFolderPath)) {
    Write-Host "Configuration folder '$exeFolderPath' does not exist. Please check your setup."
    exit 1
}

# Get the *.cpp file in the folder
$cppFile = Get-ChildItem -Path $targetPath -Filter "*.cpp" -File

if ($cppFile) {
    $cppFileName = $cppFile.Name  # Extract the file name
    $exeFileName = $cppFileName -replace ".cpp$", ".exe"  # Replace .cpp extension with .exe
    $exeFilePath = "$exeFolderPath\$exeFileName"

    # Check if the executable exists
    if (-not (Test-Path -Path $exeFilePath)) {
        Write-Host "Executable '$exeFilePath' does not exist. Please ensure it is built and available in the correct folder."
        exit 1
    }

    # Run the executable based on the specified configuration
    & $exeFilePath -window 1600x900
    # & $exeFilePath -window 1200x675
    # & $exeFilePath -fullscreen 1600x900
    # & $exeFilePath -fullscreen 2560x1440
} else {
    Write-Host "No .cpp file found in the folder."
}
