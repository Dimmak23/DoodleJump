# !/bin/bash

DIRPATH="./build"

if [ -d $DIRPATH ]; then
    echo "$DIRPATH exists..."
else
    echo "Creating $DIRPATH"
    mkdir -p build
fi

# Set-Location build ; cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Debug -G "Watcom WMake" ; Set-Location ..

cd build && cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Debug -G Ninja && cd -
# cd build && cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && cd -
# cd build && cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=Debug -G Watcom WMake" && cd -
