# ? What to grab
if(NOT UNIX)
	set(DEPENDECIES
		${VENDOR_FOLDER}/SDL2/bin
		${VENDOR_FOLDER}/SDL2_image/bin
	)
else()
	set(DEPENDECIES
		${VENDOR_FOLDER}/SDL2/lib
		${VENDOR_FOLDER}/SDL2_image/lib
	)
endif()

# ? Copy builded connector and it's dependencies to install folder
if(WIN32)
	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		set(CONNECTOR_INSTALL_DIR_BIN ${OUTPUT_DIR})
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		set(CONNECTOR_INSTALL_DIR_BIN ${OUTPUT_DIR}/${CMAKE_BUILD_TYPE})
	endif()

	message("[...]\t\tCopying SDL2 and SDL2_connector DLLs to ${CONNECTOR_INSTALL_DIR_BIN}")

	add_custom_command(
		TARGET SDL2_connector POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${DEPENDECIES}
		${CONNECTOR_INSTALL_DIR_BIN}
		COMMENT "[...]\t\tCopying SDL2 and SDL2_connector DLLs to ${CONNECTOR_INSTALL_DIR_BIN}"
	)
endif()
