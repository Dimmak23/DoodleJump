# ? Create separate library for the dynamic world
add_library(
	dynamic_world
	${LIB_TYPE}
	${GAME_INCLUDES}/level/world/dynamic/WorldDynamic.cpp
)
target_include_directories(
	dynamic_world
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	dynamic_world
	PRIVATE image
	PRIVATE locator
	PRIVATE random_spawner
	PRIVATE ammo
	PRIVATE wormhole
	PRIVATE rectangle
)
set_specific_definitions(
	dynamic_world
)
set_output_properties(dynamic_world)
