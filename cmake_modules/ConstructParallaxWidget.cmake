# ? Create separate library for the parallax widget
add_library(
	parallax_widget
	${LIB_TYPE}
	${MOD_PARALLAX}/ParallaxItem.cpp
	${MOD_PARALLAX}/ParallaxItem.hpp
)
target_include_directories(
	parallax_widget
	PUBLIC ${MOD_LOCATOR} # ? Inherit interface IRelocatableActor
	PUBLIC ${SCREEN} # ? Composes with ScreenItem*
	PUBLIC ${WR_SPRITE} # ? Compose SpriteWrapper*
	PUBLIC ${MOD_LOCATOR} # ? Compose Locator*
)
target_link_libraries(
	parallax_widget
	PRIVATE sprite_wrapper
	PRIVATE locator
	PRIVATE rectangle
)
set_specific_definitions(
	parallax_widget
)
set_output_properties(parallax_widget)
