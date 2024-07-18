# ? Copy connector and it's dependencies to executable folder
if(WIN32)
	add_custom_command(
		TARGET ${MAIN} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${VC_BIN}
		${APP_OUTPUT_DIR}
		COMMENT "[DoodleJump game]\t\t\tCopying SDL2 and SDL2_connector DLLs to ${APP_OUTPUT_DIR}..."
	)
endif()
