# !/bin/bash

DIRPATH="./bin"

if [ -d $DIRPATH ]; then
    echo "Deleting $DIRPATH"
	rm -rf $DIRPATH
else
    echo "Directory $DIRPATH doesn't exist..."
fi
