# !/bin/bash

DIRPATH="./build"

if [ -d $DIRPATH ]; then
    echo "Deleting $DIRPATH"
	rm -rf build
else
    echo "Directory $DIRPATH doesn't exist..."
fi
