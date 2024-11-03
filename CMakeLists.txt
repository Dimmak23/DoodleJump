# ? Selecting C++20 through CMAKE_CXX_STANDARD requires 3.12
cmake_minimum_required(VERSION 3.12.0)
project(DOODLE_JUMP VERSION 2.0.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ! Questionable
# ? Need to have static libs to link along with dynamic libraries
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# ? Let's not use STATIC libraries for now,
# ? this approach could unify build process for vendor, connector and game
set(LIB_TYPE "SHARED")

# ? Where we can find cmake modules
set(CMAKE_MODS "${CMAKE_SOURCE_DIR}/cmake_modules")

# ? Need to define preprocessors
include("${CMAKE_MODS}/AddCompileDefinitions.cmake")

# ? Choose to open terminal when debugging and get rid of it on release
include("${CMAKE_MODS}/AddLinkerFlags.cmake")

# ? Checking if everything correct
message("CMAKE_EXE_LINKER_FLAGS: " "${CMAKE_EXE_LINKER_FLAGS}.")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# TODO: Create cmake FindSDL2Connector.cmake module to find connector
# TODO: later SDL2_CONC::SDL2_CONC may be more robust to use
# TODO: and also maybe not needed later to include headers
set(VC_EXPORT_INCLUDE "${CMAKE_SOURCE_DIR}/SDL2_connector/include") # ! This is for Game app
set(VC_BIN "${CMAKE_SOURCE_DIR}/SDL2_connector/bin/${CMAKE_BUILD_TYPE}") # ! This is for Game app

# set(VC_LIB "${CMAKE_SOURCE_DIR}/lib") # ! temporally unused

# ? Specify where get SDL2_connector library
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(SDL2_Connector_LIBRARY "${VC_BIN}/libSDL2_connector.dll")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(SDL2_Connector_LIBRARY "${VC_BIN}/SDL2_connector.lib") # ! You know what? dll also will be linked
endif()

message("...[library_path]: ${SDL2_Connector_LIBRARY}")

# ? Actual usage will be kept for every target in the separate file
# ? 'targets' - all targets here
set(MAIN DoodleJump)

# * Icon
set(APP_ICON_WIN "${CMAKE_SOURCE_DIR}/resources/icon.rc")

# ? Set up paths for the custom libraries headers
set(GAME_INCLUDES "${CMAKE_SOURCE_DIR}/game/libraries")

# ? All libraries and main executable would be created with output properties
# ? so tell how all of them output
include("${CMAKE_MODS}/OutputProperties.cmake")

# ? Apply changes for MSVC compiler
include("${CMAKE_MODS}/MSVCSpecials.cmake")

# ? Build all libraries needed
include("${CMAKE_MODS}/BuildLibraries.cmake")

# ? Set specific compiler definitions
include("${CMAKE_MODS}/AddCompileDefinitions.cmake")

# ? Link game application to the source cpp - building the game that way
include("${CMAKE_MODS}/BuildGame.cmake")

# TODO: send this to the cmake module
# ? Copy SDL2_connector and it's dependencies
add_custom_command(
	TARGET ${MAIN} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${VC_BIN} # ? Copy dll's from SDL2_connector
	$<TARGET_FILE_DIR:${MAIN}> # ? To where game.exe leave
	COMMENT "Copying app's DLLs to $<TARGET_FILE_DIR:${MAIN}>"
)

# ? Game executable would need assets folder at the same directory
include("${CMAKE_MODS}/ExtractAssets.cmake")

# TODO: send this to the cmake module
# ? This is used for Microsoft Visual Studio IDE: need 'assets' folder in the same folder with game.sln
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/data/")
		add_custom_command(
			TARGET ${MAIN} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/data
			${CMAKE_BINARY_DIR}/data
		)
	endif()
endif()
