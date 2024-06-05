# ? Game executable would need assets folder at the same directory
# ? Export assets to the main executable folder
if(NOT EXISTS "${OUTPUT_DIR}/data/")
	add_custom_command(
		TARGET ${MAIN} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/data
		$<TARGET_FILE_DIR:${MAIN}>/data
	)
endif()
