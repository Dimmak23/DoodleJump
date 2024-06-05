# I would like to keep every target binary in it's own dir,
# also 'Debug' and 'Release' dir's inside them as well
# 'executables' - all executables root
set(EXECUTABLE_ROOT "${CMAKE_SOURCE_DIR}/executables")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set(APP_OUTPUT_DIR "${EXECUTABLE_ROOT}/Debug")
	else()
		set(APP_OUTPUT_DIR "${EXECUTABLE_ROOT}/Release")
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(APP_OUTPUT_DIR ${EXECUTABLE_ROOT})
endif()

# ? Define a function to set common target properties
function(set_output_properties TARGET)
	if(LIB_TYPE MATCHES "STATIC")
		set_target_properties(
			${TARGET}
			PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "${APP_OUTPUT_DIR}" # ? Binary
			ARCHIVE_OUTPUT_DIRECTORY "${APP_OUTPUT_DIR}" # ? Static libraries
		)
	elseif(LIB_TYPE MATCHES "SHARED")
		set_target_properties(
			${TARGET}
			PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "${APP_OUTPUT_DIR}" # ? Binary
			LIBRARY_OUTPUT_DIRECTORY "${APP_OUTPUT_DIR}" # ? Dynamic libraries
		)
	endif()
endfunction()
