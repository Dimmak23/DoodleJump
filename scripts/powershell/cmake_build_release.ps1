# $targetPath = ".\source"

# # Get the *.cpp file in the folder
# $cppFile = Get-ChildItem -Path $targetPath -Filter "*.cpp" -File

# if ($cppFile) {
#     $cppFileName = $cppFile.Name  # Extract the file name
#     $targetFileName = $cppFileName -replace ".cpp$", ""  # Replace .cpp extension with blank space
# } else {
#     Write-Host "No .cpp file found in the folder."
# }

# cmake --build build --config Release --target $targetFileName -j 8 -v

# cmake --build build --config Release --target $targetFileName -j 8 -v
cmake --build build --config Release -j 8 -v
# cmake --build build --config Release --target SDL2_connector -j 8 -v
# cmake --build build --config Release --target TL -j 8 -v
# cmake --build build --config Release --target SDL2_connector TL -j 8 -v