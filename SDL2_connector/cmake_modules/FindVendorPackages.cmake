# ? specify support for find_package()
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(SDL2_DIR "${VENDOR_FOLDER}/SDL2/lib/cmake/SDL2") # ? Where SDL2?
	set(SDL2_image_DIR "${VENDOR_FOLDER}/SDL2_image/lib/cmake/SDL2_image") # ? Where SDL2_image?
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(SDL2_DIR "${VENDOR_FOLDER}/SDL2/cmake")
	set(SDL2_image_DIR "${VENDOR_FOLDER}/SDL2_image/cmake")
endif()

message("looking for SDL2 cmake modules: ${SDL2_DIR}")
message("looking for SDL2_image cmake modules: ${SDL2_IMAGE_DIR}")

# ? Set the installation directory for jpeg
set(JPEG_INSTALL_DIR "${VENDOR_FOLDER}/JPEG")
set(JPEG_INCLUDE_DIR "${JPEG_INSTALL_DIR}/include")

if(NOT UNIX)
	set(JPEG_LIBRARY "${JPEG_INSTALL_DIR}/bin")
else()
	set(JPEG_LIBRARY "${JPEG_INSTALL_DIR}/lib")
endif()


# ? Set the installation directory for png
set(PNG_INSTALL_DIR "${VENDOR_FOLDER}/PNG")
set(PNG_PNG_INCLUDE_DIR "${PNG_INSTALL_DIR}/include")

if(NOT UNIX)
	set(PNG_LIBRARY "${PNG_INSTALL_DIR}/bin")
else()
	set(PNG_LIBRARY "${PNG_INSTALL_DIR}/lib")
endif()

# ? Find SDL2 package
find_package(SDL2 REQUIRED)

# ? Find SDL2_image package
find_package(SDL2_image REQUIRED)
