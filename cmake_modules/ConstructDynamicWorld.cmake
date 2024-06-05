# ? Create separate library for the dynamic world
add_library(
	dynamic_world
	${LIB_TYPE}
	${LVL_DWORLD}/WorldDynamic.cpp
	${LVL_DWORLD}/WorldDynamic.hpp
)
target_include_directories(
	dynamic_world
	PUBLIC ${LVL_MAIN} # ? Compose with IStoppable*
	PUBLIC ${SCREEN} # ? Composes with ScreenItem*
	PUBLIC ${WR_SPRITE} # ? To spawn SpriteWrapper*
	PUBLIC ${MOD_LOCATOR}
	PUBLIC ${MOD_SCENE}
	PUBLIC ${MOD_COLLIDER}
	PUBLIC ${MOD_RSPAWN}
	PUBLIC ${UNI_FEATURES} # ? IUniverse
	PUBLIC ${RECT} # ? Composes with RectangleCore*
	PUBLIC ${UTILS}
	PUBLIC ${PT_AMMO} # ? Composes with Ammo*
	PUBLIC ${ABL_WRMHL} # ? Composes with Wormhole*
)
target_link_libraries(
	dynamic_world
	PRIVATE sprite_wrapper
	PRIVATE locator
	PRIVATE random_spawner
	PRIVATE ammo
	PRIVATE wormhole
	PRIVATE rectangle
)
set_specific_definitions(
	dynamic_world
)
set_output_properties(dynamic_world)
