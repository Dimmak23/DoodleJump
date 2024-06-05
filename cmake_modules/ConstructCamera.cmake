# ? Create separate library for the camera
add_library(
	camera
	${LIB_TYPE}
	${MOD_CAMERA}/Camera.cpp
	${MOD_CAMERA}/Camera.hpp
)
target_include_directories(
	camera
	PUBLIC ${SCREEN} # ? Composes with ScreenItem*
	PUBLIC ${MOD_PENGINE}
	PUBLIC ${MOD_LOCATOR}
	PUBLIC ${MOD_SCENE}
	PUBLIC ${WR_SPRITE} # ? Composes with SpriteWrapper*
)
target_link_libraries(
	camera
	PRIVATE physics_engine
	PRIVATE locator
	PRIVATE sprite_wrapper
	PRIVATE rectangle
)
set_specific_definitions(
	camera
)
set_output_properties(camera)
