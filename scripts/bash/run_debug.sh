# !/bin/bash

targetPath="./tests/launch_connector"

# Get the *.cpp file in the folder
cppFile=$(find $targetPath -type f -name "*.cpp" | head -n 1)
name="$(basename -- "$cppFile" .cpp)"

if [ -n "$name" ]; then
    ./executables/Debug/$name -window 800x600
    # & "./executables/Debug/$cppFile" -window 1200x675
    # & "./executables/Debug/$cppFile" -window 1600x900
    # & "./executables/Debug/$cppFile" -fullscreen 1600x900
    # & "./executables/Debug/$cppFile" -fullscreen 2560x1440
else
    echo "No .cpp file found in the folder."
fi
