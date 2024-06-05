# $targetPath = ".\source"
$targetPath = ".\tests\launch_connector"

# Get the *.cpp file in the folder
$cppFile = Get-ChildItem -Path $targetPath -Filter "*.cpp" -File

if ($cppFile) {
    $cppFileName = $cppFile.Name  # Extract the file name
    $exeFileName = $cppFileName -replace ".cpp$", ".exe"  # Replace .cpp extension with .exe
    # & ".\executables\Release\$exeFileName" -window 800x600
    # & ".\executables\Release\$exeFileName" -window 1200x675
    # & ".\executables\Release\$exeFileName" -window 1600x900
    & ".\executables\Release\$exeFileName" -fullscreen 2560x1440
} else {
    Write-Host "No .cpp file found in the folder."
}
