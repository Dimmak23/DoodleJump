if(WIN32)
	add_executable(
		${MAIN}
		WIN32
		${CMAKE_SOURCE_DIR}/game/source/${MAIN}.cpp
		"${APP_ICON_WIN}"
	)
else()
	add_executable(
		${MAIN}
		${CMAKE_SOURCE_DIR}/game/source/${MAIN}.cpp
	)
endif()

target_include_directories(
	${MAIN}
	PUBLIC ${LIB_APPLICATION}
)
target_link_libraries(
	${MAIN}
	PRIVATE application
)
set_specific_definitions(
	${MAIN}
)
set_output_properties(${MAIN})
