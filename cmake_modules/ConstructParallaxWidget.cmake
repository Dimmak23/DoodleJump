# ? Create separate library for the parallax widget
add_library(
	parallax_widget
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/parallax/ParallaxItem.cpp
)
target_include_directories(
	parallax_widget
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	parallax_widget
	PRIVATE image
	PRIVATE locator
	PRIVATE rectangle
)
set_specific_definitions(
	parallax_widget
)
set_output_properties(parallax_widget)
