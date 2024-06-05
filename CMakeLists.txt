# ? Selecting C++20 through CMAKE_CXX_STANDARD requires 3.12
cmake_minimum_required(VERSION 3.12.0)
project(DOODLE_JUMP VERSION 1.2.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ? Where we can find cmake modules
set(CMAKE_MODS "${CMAKE_SOURCE_DIR}/cmake_modules")

# ? Need to define preprocessors
include("${CMAKE_MODS}/AddCompileDefinitions.cmake")

# ? Choose to open terminal when debugging and get rid of it on release
include("${CMAKE_MODS}/AddLinkerFlags.cmake")

# ! Questionable
# ? Need to have static libs to link along with dynamic libraries
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# ? Checking if everything correct
message(STATUS "CMAKE_EXE_LINKER_FLAGS: " "${CMAKE_EXE_LINKER_FLAGS}.")
message(STATUS "Libraries type: " ${LIB_TYPE})

# TODO: Tell this to the Game app
set(VC_EXPORT_INCLUDE "${CMAKE_SOURCE_DIR}/SDL2_connector/include") # ! This is for Game app
set(VC_BIN "${CMAKE_SOURCE_DIR}/SDL2_connector/bin/${CMAKE_BUILD_TYPE}") # ! This is for Game app

# set(VC_LIB "${CMAKE_SOURCE_DIR}/lib") # ! temporally unused

# ? Specify where get SDL2_connector library
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	if(UNIX)
		set(SDL2_Connector_LIBRARY "${VC_BIN}/libSDL2_connector.so")
	else()
		set(SDL2_Connector_LIBRARY "${VC_BIN}/libSDL2_connector.dll")
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(SDL2_Connector_LIBRARY "${VC_BIN}/SDL2_connector.lib") # ! You know what? dll also will be linked
endif()

message("...[library_path]: ${SDL2_Connector_LIBRARY}")

set(MAIN TL)
set(TEST_FOLDER "${CMAKE_SOURCE_DIR}/tests/launch_connector")
set(TEST_APP_FOLDER "${CMAKE_SOURCE_DIR}/tests/tested_application")

# ? Path to the twoDarKade:
set(TWO_D_ARKADE_INCLUDE "${CMAKE_SOURCE_DIR}/game/libraries")

# ? Rectangle type and features
add_library(
	DK_Rectangle
	${LIB_TYPE}
	${TWO_D_ARKADE_INCLUDE}/rectangle/RectangleShape.cpp
	${TWO_D_ARKADE_INCLUDE}/rectangle/RectangleCore.cpp
)
target_include_directories(
	DK_Rectangle
	PUBLIC ${TWO_D_ARKADE_INCLUDE}
)

# ? Need to extract build
include("${CMAKE_MODS}/OutputProperties.cmake")
set_output_properties(DK_Rectangle)

# ? Fix MSVC compiler warnings
include("${CMAKE_MODS}/MSVCSpecials.cmake")
set_specific_definitions(DK_Rectangle)

# ? Image wrapper of sprite
add_library(
	DK_Image
	${LIB_TYPE}
	${TWO_D_ARKADE_INCLUDE}/graphics/image/Image.cpp
)
target_include_directories(
	DK_Image
	PUBLIC ${VC_EXPORT_INCLUDE}
	PUBLIC ${TWO_D_ARKADE_INCLUDE}
)
target_link_libraries(
	DK_Image
	PUBLIC "${SDL2_Connector_LIBRARY}"
	PUBLIC DK_Rectangle
)
set_output_properties(DK_Image)
set_specific_definitions(DK_Image)

# ? Testing window creation ability
add_executable(
	${MAIN}
	${TEST_FOLDER}/${MAIN}.cpp
	${TEST_APP_FOLDER}/TestedGameApp.cpp
)
target_include_directories(
	${MAIN}
	PRIVATE ${VC_EXPORT_INCLUDE}
	PRIVATE ${TEST_APP_FOLDER}
	PRIVATE ${TWO_D_ARKADE_INCLUDE}
)
target_link_libraries(
	${MAIN}
	PRIVATE "${SDL2_Connector_LIBRARY}"
	PRIVATE DK_Image
)
set_output_properties(${MAIN})
set_specific_definitions(${MAIN})

# ? Need to compile ${MAIN} with flags
include("${CMAKE_MODS}/AddCompilerFlags.cmake")
set_compiler_flags(${MAIN})

# ? Copy SDL2_connector and it's dependencies
add_custom_command(
	TARGET ${MAIN} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${VC_BIN} # ? Copy dll's from SDL2_connector
	$<TARGET_FILE_DIR:${MAIN}> # ? To where game.exe leave
	COMMENT "Copying app's DLLs to $<TARGET_FILE_DIR:${MAIN}>"
)

# ? Game executable would need assets folder at the same directory
if(NOT EXISTS "$<TARGET_FILE_DIR:${MAIN}>/assets/")
	add_custom_command(
		TARGET ${MAIN} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets
		$<TARGET_FILE_DIR:${MAIN}>/assets
	)
endif()

# ? This is used for Microsoft Visual Studio IDE: need 'assets' folder in the same folder with game.sln
if(NOT EXISTS "${CMAKE_BINARY_DIR}/assets/")
	add_custom_command(
		TARGET ${MAIN} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets
		${CMAKE_BINARY_DIR}/assets
	)
endif()

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
