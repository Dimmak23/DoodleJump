# ? Game executable would need shared libraries at the same directory
if(WIN32)
	set(framework_libraries
		${Framework_LIBRARY}/FrameworkRelease_x64.dll
		${Framework_LIBRARY}/libjpeg-9.dll
		${Framework_LIBRARY}/libpng16-16.dll
		${Framework_LIBRARY}/SDL2_image.dll
		${Framework_LIBRARY}/SDL2.dll
		${Framework_LIBRARY}/zlib1.dll
	)

	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		set(destination_folder ${OUTPUT_DIR})
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		set(destination_folder ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE})
	endif()

	add_custom_command(
		TARGET ${MAIN}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${framework_libraries} ${destination_folder}
		COMMENT "Copying DLLs to ${destination_folder}"
	)
endif()
