# ? Selecting C++20 through CMAKE_CXX_STANDARD requires 3.12
cmake_minimum_required(VERSION 3.12.0)
project(INSTALL_SDL2_CONNECTOR VERSION 1.2.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ! Questionable
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# ? Enable debugging LogLine from Loger.hpp
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_definitions(-D_DEBUG)
endif()

# ! where to find all installed vendors: ${VENDOR_FOLDER}
set(VENDOR_FOLDER "${CMAKE_SOURCE_DIR}/../${VENDOR_FOLDER}")

# ? Where we can find sintructions to install SDL dependencies
set(CONNECTOR_MODS "${CMAKE_SOURCE_DIR}/cmake_modules")

# ? Choose to open terminal when debugging and get rid of it on release
include("${CONNECTOR_MODS}/AddLinkerFlags.cmake")

# ? Find and connect vendor libraries
include("${CONNECTOR_MODS}/FindVendorPackages.cmake")

# ? Where vendor connector leaves
set(VC_SRC "${CMAKE_SOURCE_DIR}/source")
set(VC_IMPORT_INCLUDE "${CMAKE_SOURCE_DIR}/include/SDL2_Connector")

# ? Create connector
add_library(
	SDL2_connector
	SHARED
	${VC_SRC}/Render.cpp
	${VC_SRC}/Sprite.cpp
	${VC_SRC}/Launcher.cpp
)
target_include_directories(
	SDL2_connector
	PUBLIC ${VENDOR_FOLDER}/SDL2/include/SDL2
	PUBLIC ${VENDOR_FOLDER}/SDL2_image/include/SDL2_image
	PUBLIC ${VC_IMPORT_INCLUDE}
)
target_link_libraries(
	SDL2_connector
	PUBLIC SDL2::SDL2
	PUBLIC SDL2_image::SDL2_image
)

# ? Where to install
include("${CONNECTOR_MODS}/OutputProperties.cmake")
set_output_properties(SDL2_connector)

# ? Add compile defenitions
include("${CONNECTOR_MODS}/AddMVSCSpecialCommands.cmake")
set_specific_definitions(SDL2_connector)

# ? Additional compiler flags
include("${CONNECTOR_MODS}/AddCompilerFlags.cmake")
set_compiler_flags(SDL2_connector)

# ? Exporting symbols for any game app
target_compile_definitions(
	SDL2_connector
	PUBLIC BUILD_CONNECTOR_API
)

# ? Finally install connector
include("${CONNECTOR_MODS}/InstallSDL2Connector.cmake")

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
