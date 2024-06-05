# ? Create separate library for the static world
add_library(
	static_world
	${LIB_TYPE}
	${LVL_SWORLD}/WorldStatic.cpp
	${LVL_SWORLD}/WorldStatic.hpp
)
target_include_directories(
	static_world
	PUBLIC ${SCREEN} # ? Composes with ScreenItem*
	PUBLIC ${WR_SPRITE} # ? To spawn SpriteWrapper*
	PUBLIC ${MOD_LOCATOR}
	PUBLIC ${MOD_SCENE}
	PUBLIC ${MOD_COLLIDER}
	PUBLIC ${MOD_RSPAWN}
	PUBLIC ${UNI_FEATURES} # ? IUniverse
	PUBLIC ${RECT} # ? Using methods from RectangleCore
	PUBLIC ${ABL_STEALTH} # ? Composes with Stealth*
	PUBLIC ${UTILS}
)
target_link_libraries(
	static_world
	PRIVATE sprite_wrapper
	PRIVATE locator
	PRIVATE random_spawner
	PRIVATE stealth
	PRIVATE rectangle
)
set_specific_definitions(
	static_world
)
set_output_properties(static_world)
