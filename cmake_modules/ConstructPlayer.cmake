# ? Create separate library for the player actor
add_library(
	player
	${LIB_TYPE}
	${GAME_INCLUDES}/actor/player/Player.cpp
)
target_include_directories(
	player
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	player
	PRIVATE animated_image
	PRIVATE locator
	PRIVATE physics_engine
	PRIVATE rectangle
)
set_specific_definitions(
	player
)
set_output_properties(player)
