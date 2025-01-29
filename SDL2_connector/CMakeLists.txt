# ? While FetchContent only requires CMake 3.11, selecting C++20 through
# ? CMAKE_CXX_STANDARD requires 3.12 and the convenience function
# ? FetchContent_MakeAvailable was introduced in CMake 3.14
cmake_minimum_required(VERSION 3.14.0)
project(SDL2_connector VERSION 2.2.0)

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

# ? Where we can find sintructions to install SDL dependencies
set(CONNECTOR_MODS "${CMAKE_SOURCE_DIR}/cmake_modules")

# ? Choose to open terminal when debugging and get rid of it on release
include("${CONNECTOR_MODS}/AddLinkerFlags.cmake")

# TODO: This is something to implement for SDL_mixer and SDL_ttf in the future
# set(DEPS "${CMAKE_SOURCE_DIR}/dependencies")
# set(DEPS_MODS "${DEPS}/cmake_modules")

# ! First of all make sure that some of the dependencies are built from source
# add_subdirectory(dependencies)

# ! Find all needed dependency fiels
# include("${CONNECTOR_MODS}/FindDependencies.cmake")
# TODO: This is something to implement for SDL_mixer and SDL_ttf in the future

# ? Find and connect vendor libraries
include("${CONNECTOR_MODS}/FetchSDL2Packages.cmake")


# ? Where vendor connector leaves
set(VC_SRC "${CMAKE_SOURCE_DIR}/source")
set(VC_IMPORT_INCLUDE "${CMAKE_SOURCE_DIR}/include/SDL2_connector")

# ? Create connector
add_library(
	${PROJECT_NAME}
	SHARED
	${VC_SRC}/Render.cpp
	${VC_SRC}/Sprite.cpp
	${VC_SRC}/Launcher.cpp
)
target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${VC_IMPORT_INCLUDE}
	PUBLIC ${CMAKE_BIN_DIR}/_deps/sdl2-src/include
	PUBLIC ${CMAKE_BIN_DIR}/_deps/sdl2_image-src/include
)
target_link_libraries(
	${PROJECT_NAME}
	PUBLIC SDL2::SDL2
	PUBLIC SDL2_image::SDL2_image
	# PUBLIC SDL2_mixer::SDL2_mixer # TODO: for the future
	# PUBLIC SDL2_ttf::SDL2_ttf # TODO: for the future
)

# ? Where to install
set(VC_BINARY "${CMAKE_SOURCE_DIR}/bin")
include("${CONNECTOR_MODS}/OutputProperties.cmake")
set_output_properties(${PROJECT_NAME})

# ? Add compile defenitions
include("${CONNECTOR_MODS}/AddMVSCSpecialCommands.cmake")
set_specific_definitions(${PROJECT_NAME})

# ? Additional compiler flags
include("${CONNECTOR_MODS}/AddCompilerFlags.cmake")
set_compiler_flags(${PROJECT_NAME})

# ? Exporting symbols for any game app
target_compile_definitions(
	${PROJECT_NAME}
	PUBLIC BUILD_CONNECTOR_API
)

# ? Finally install connector for Windows
# ? For Linux output properties would be enough 
include("${CONNECTOR_MODS}/ExtractSDL2Packages.cmake")
