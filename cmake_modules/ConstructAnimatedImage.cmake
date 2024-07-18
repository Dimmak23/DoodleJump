# ? Create separate library for the animated sprite wrapper
add_library(
	animated_image
	${LIB_TYPE}
	${GAME_INCLUDES}/graphics/animated_image/AnimatedImage.cpp
)
target_include_directories(
	animated_image
	PUBLIC ${VC_EXPORT_INCLUDE}
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	animated_image
	PRIVATE image
)
set_specific_definitions(
	animated_image
)
set_output_properties(animated_image)
