# ? Create separate library for the scene module
add_library(
	scene
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/scene/Scene.cpp
)
target_include_directories(
	scene
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	scene
	PRIVATE locator
)
set_specific_definitions(
	scene
)
set_output_properties(scene)
