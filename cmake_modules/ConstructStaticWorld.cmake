# ? Create separate library for the static world
add_library(
	static_world
	${LIB_TYPE}
	${GAME_INCLUDES}/level/world/static/WorldStatic.cpp
)
target_include_directories(
	static_world
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	static_world
	PRIVATE image
	PRIVATE locator
	PRIVATE random_spawner
	PRIVATE stealth
	PRIVATE rectangle
)
set_specific_definitions(
	static_world
)
set_output_properties(static_world)
