# ? Selecting C++20 through CMAKE_CXX_STANDARD requires 3.12
cmake_minimum_required(VERSION 3.12.0)
project(INSTALL_SDL2_DEPS VERSION 1.2.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ! Questionable
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# ? Where we can find sintructions to install SDL dependencies
set(VENDOR_MODS "${CMAKE_SOURCE_DIR}/cmake_modules")

# ? Set the installation directory for SDL
set(SDL_INSTALL_DIR "${CMAKE_SOURCE_DIR}/SDL2")
set(SDL_INCLUDE "${SDL_INSTALL_DIR}/include/SDL2")
set(SDL_BINARY "${SDL_INSTALL_DIR}/bin")
set(SDL_LIB "${SDL_INSTALL_DIR}/lib")

# ? Set the installation directory for SDL_image
set(SDL_IMAGE_INSTALL_DIR "${CMAKE_SOURCE_DIR}/SDL2_image")
set(SDL_IMAGE_INCLUDE "${SDL_IMAGE_INSTALL_DIR}/include/SDL2")
set(SDL_IMAGE_BINARY "${SDL_IMAGE_INSTALL_DIR}/bin")
set(SDL_IMAGE_LIB "${SDL_IMAGE_INSTALL_DIR}/lib")

# ? Set the installation directory for jpeg
set(JPEG_INSTALL_DIR "${CMAKE_SOURCE_DIR}/JPEG")
set(JPEG_INCLUDE_DIR "${JPEG_INSTALL_DIR}/include")
set(JPEG_LIBRARY "${JPEG_INSTALL_DIR}/bin")

# ? Set the installation directory for png
set(PNG_INSTALL_DIR "${CMAKE_SOURCE_DIR}/PNG")
set(PNG_INCLUDE_DIR "${PNG_INSTALL_DIR}/include")
set(PNG_LIBRARY "${PNG_INSTALL_DIR}/bin")

# ? Utility to add vendors
include(ExternalProject)

# ? Install SDL2
include("${VENDOR_MODS}/InstallSDL2.cmake")

# ! some PC's would need PkgConfig also (possible place to found: L:/msys64/mingw64/bin/pkg-config.exe)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	# ! some PC's would need ZLIB also (possible place to found: C:/Program Files (x86)/zlib/lib/zlib.lib)
	# ! or /usr/lib/x86_64-linux-gnu/libz.so

	# ? SDL2_image would need jpeg
	include("${VENDOR_MODS}/InstallJPEG.cmake")

	# ? SDL2_image would need png
	include("${VENDOR_MODS}/InstallPNG.cmake")
endif()

# ? Install SDL2_image
include("${VENDOR_MODS}/InstallSDL2_Image.cmake")

# ? Create the content for the text file
set(RESULT_STAMP_FILE ${CMAKE_SOURCE_DIR}/build_info.txt)
set(CONTENT "Build completed at:")
string(TIMESTAMP CURRENT_TIMESTAMP "%Y-%m-%d %H:%M:%S")
set(CONTENT "${CONTENT} ${CURRENT_TIMESTAMP}\nBuild type: ${CMAKE_BUILD_TYPE}")
set(CONTENT "${CONTENT}\nGenerator was used: ${CMAKE_GENERATOR}")
set(CONTENT "${CONTENT}\nC compiler used: ${CMAKE_C_COMPILER}")
set(CONTENT "${CONTENT}\nC++ compiler used: ${CMAKE_CXX_COMPILER}")

# ? Create the text file
file(WRITE "${RESULT_STAMP_FILE}" "${CONTENT}")
