# ? Create separate library for the sprite wrapper
add_library(
	sprite_wrapper
	${LIB_TYPE}
	${WR_SPRITE}/SpriteWrapper.cpp
	${WR_SPRITE}/SpriteWrapper.hpp
)
target_include_directories(
	sprite_wrapper
	PUBLIC ${Framework_INCLUDE_PATH}
	PUBLIC ${RECT} # ? Inherit from RectangleShape
	PUBLIC ${WR_FEATURES} # ? Inherit from IGraphicle
)
target_link_libraries(
	sprite_wrapper
	PUBLIC "${Framework_BINARY}"
	PRIVATE rectangle # ? Inherit from RectangleShape
)
set_specific_definitions(
	sprite_wrapper
)
set_output_properties(sprite_wrapper)
