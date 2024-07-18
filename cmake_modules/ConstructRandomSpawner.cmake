# ? Create separate library for the random_spawner
add_library(
	random_spawner
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/spawn/RandomSpawn.cpp
)
target_include_directories(
	random_spawner
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	random_spawner
	PRIVATE rectangle
)
set_specific_definitions(
	random_spawner
)
set_output_properties(random_spawner)
