rm -rf executables

JPEGPATH="./JPEG"
PNGPATH="./PNG"
SDL2PATH="./SDL2"
SDL2_imagePATH="./SDL2_image"

if [ -d $JPEGPATH ]; then
    echo "Deleting $JPEGPATH"
	rm -rf $JPEGPATH
else
    echo "Directory: $JPEGPATH doesn't exist."
fi

if [ -d $PNGPATH ]; then
    echo "Deleting $PNGPATH"
	rm -rf $PNGPATH
else
    echo "Directory: $PNGPATH doesn't exist."
fi

if [ -d $SDL2PATH ]; then
    echo "Deleting $SDL2PATH"
	rm -rf $SDL2PATH 
else
    echo "Directory: $SDL2PATH doesn't exist."
fi

if [ -d $SDL2_imagePATH ]; then
    echo "Deleting $SDL2_imagePATH"
	rm -rf $SDL2_imagePATH
else
    echo "Directory: $SDL2_imagePATH doesn't exist."
fi

file_path="./build_info.txt"

if [ -f "$file_path" ]; then
    echo "Deleting $file_path ..."
else
    echo "$file_path does not exist."
fi