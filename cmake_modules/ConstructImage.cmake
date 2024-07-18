# ? Create separate library for the sprite wrapper
add_library(
	image
	${LIB_TYPE}
	${GAME_INCLUDES}/graphics/image/Image.cpp
)
target_include_directories(
	image
	PUBLIC ${VC_EXPORT_INCLUDE}
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	image
	PUBLIC "${SDL2_Connector_LIBRARY}"
	PRIVATE rectangle # ? Inherit from RectangleShape
)
set_specific_definitions(
	image
)
set_output_properties(image)
