# ? Create separate library for the game application
add_library(
	application
	${LIB_TYPE}
	${LIB_APPLICATION}/GameApplication.cpp
	${LIB_APPLICATION}/GameApplication.hpp
)
target_include_directories(
	application
	PUBLIC ${Framework_INCLUDE_PATH}
	PUBLIC ${LVL_MAIN}
	PUBLIC ${UTILS}
	PUBLIC ${RECT} # ? Using Point*
)
target_link_libraries(
	application
	PUBLIC "${Framework_BINARY}"
	PRIVATE level
)
set_specific_definitions(
	application
)
set_output_properties(application)
