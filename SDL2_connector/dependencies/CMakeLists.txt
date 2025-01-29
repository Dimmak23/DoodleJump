project(LOAD_DEPS VERSION 0.0.1)

# ? Utility to add dependencies
include(ExternalProject)

# ! some PC's would need PkgConfig also (possible place to found: L:/msys64/mingw64/bin/pkg-config.exe)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # ! some PC's would need ZLIB also (possible place to found: C:/Program Files (x86)/zlib/lib/zlib.lib)

  # ? SDL2_image would need JPEG
  set(JPEG_INSTALL_DIR "${DEPS}/JPEG")
  set(JPEG_INCLUDE_DIR "${JPEG_INSTALL_DIR}/include")
  set(JPEG_LIBRARY "${JPEG_INSTALL_DIR}/bin")

  # ? SDL2_image would need PNG
  set(PNG_INSTALL_DIR "${DEPS}/PNG")
  set(PNG_PNG_INCLUDE_DIR "${PNG_INSTALL_DIR}/include")
  set(PNG_LIBRARY "${PNG_INSTALL_DIR}/bin")

  # ? SDL2_image would need jpeg
  include("${DEPS_MODS}/InstallJPEG.cmake")

  # ? SDL2_image would need png
  include("${DEPS_MODS}/InstallPNG.cmake")
endif()
