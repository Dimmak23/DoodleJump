# ? Create separate library for the camera
add_library(
	camera
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/camera/Camera.cpp
)
target_include_directories(
	camera
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	camera
	PRIVATE physics_engine
	PRIVATE locator
	PRIVATE image
	PRIVATE rectangle
)
set_specific_definitions(
	camera
)
set_output_properties(camera)
