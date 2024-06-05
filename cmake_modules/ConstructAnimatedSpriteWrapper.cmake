# ? Create separate library for the animated sprite wrapper
add_library(
	animated_character
	${LIB_TYPE}
	${WR_ANISPRITE}/AnimatedSpriteWrapper.cpp
	${WR_ANISPRITE}/AnimatedSpriteWrapper.hpp
)
target_include_directories(
	animated_character
	PUBLIC ${Framework_INCLUDE_PATH}
	PUBLIC ${WR_SPRITE} # ? Inherit from SpriteWrapper
)
target_link_libraries(
	animated_character
	PRIVATE sprite_wrapper
)
set_specific_definitions(
	animated_character
)
set_output_properties(animated_character)
