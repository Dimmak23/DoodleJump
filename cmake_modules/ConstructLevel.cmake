# ? Create separate library for the main level
add_library(
	level
	${LIB_TYPE}
	${GAME_INCLUDES}/level/level/Level.cpp
)
target_include_directories(
	level
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	level
	PRIVATE player
	PRIVATE jet_pack
	PRIVATE parallax_widget
	PRIVATE collider
	PRIVATE camera
	PRIVATE scene
	PRIVATE score_board
	PRIVATE static_world
	PRIVATE dynamic_world
	PRIVATE gun
	PRIVATE rectangle
)
set_specific_definitions(
	level
)
set_output_properties(level)
