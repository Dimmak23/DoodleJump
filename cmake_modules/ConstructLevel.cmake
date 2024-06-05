# ? Create separate library for the main level
add_library(
	level
	${LIB_TYPE}
	${LVL_MAIN}/Level.cpp
	${LVL_MAIN}/Level.hpp
)
target_include_directories(
	level
	PUBLIC ${LVL_MAIN}
	PUBLIC ${ACT_PLAYER}
	PUBLIC ${ABL_JETPACK} # ? Jet Pack ability
	PUBLIC ${MOD_PARALLAX}
	PUBLIC ${MOD_COLLIDER}
	PUBLIC ${MOD_CAMERA}
	PUBLIC ${MOD_SCENE}
	PUBLIC ${SCR_BOARD}
	PUBLIC ${LVL_SWORLD}
	PUBLIC ${LVL_DWORLD}
	PUBLIC ${SCREEN} # ? Inherit from ScreenItem interface
	PUBLIC ${UTILS}
	PUBLIC ${MOD_GUN}
)
target_link_libraries(
	level
	PRIVATE player
	PRIVATE jet_pack
	PRIVATE parallax_widget
	PRIVATE collider
	PRIVATE camera
	PRIVATE scene
	PRIVATE score_board
	PRIVATE static_world
	PRIVATE dynamic_world
	PRIVATE gun
	PRIVATE rectangle
)
set_specific_definitions(
	level
)
set_output_properties(level)
