# ? Create separate library for the random_spawner
add_library(
	random_spawner
	${LIB_TYPE}
	${MOD_RSPAWN}/RandomSpawn.cpp
	${MOD_RSPAWN}/RandomSpawn.hpp
)
target_include_directories(
	random_spawner
	PUBLIC ${RECT} # ? Use features from RectangleCore
	PUBLIC ${UNI_FEATURES} # ? Composes with IUniverse*
)
target_link_libraries(
	random_spawner
	PRIVATE rectangle
)
set_specific_definitions(
	random_spawner
)
set_output_properties(random_spawner)
