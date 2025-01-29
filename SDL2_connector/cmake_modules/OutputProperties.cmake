# ? Different platforms and modes: different folders
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set(OUTPUT_DIR "${VC_BINARY}/Debug")
	else()
		set(OUTPUT_DIR "${VC_BINARY}/Release")
	endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(OUTPUT_DIR ${VC_BINARY})
endif()

# ? Define a function to set common target properties
function(set_output_properties TARGET)
	set_target_properties(
		${TARGET}
		PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}" # ? Binary
		ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}" # ! Static libraries
		LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}" # ? Dynamic libraries
	)
endfunction()
