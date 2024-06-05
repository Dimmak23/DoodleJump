# ? Create separate library for the scene module
add_library(
	scene
	${LIB_TYPE}
	${MOD_SCENE}/Scene.cpp
	${MOD_SCENE}/Scene.hpp
)
target_include_directories(
	scene
	PUBLIC ${MOD_LOCATOR} # ? Composes with IRelocatableActor*
	PUBLIC ${RECT} # ? Composes with RectangleCore
)
target_link_libraries(
	scene
	PRIVATE locator
)
set_specific_definitions(
	scene
)
set_output_properties(scene)
