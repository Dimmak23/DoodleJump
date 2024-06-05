# ? Create separate library for the jet pack ability
add_library(
	wormhole
	${LIB_TYPE}
	${ABL_WRMHL}/Wormhole.cpp
	${ABL_WRMHL}/Wormhole.hpp
)
target_include_directories(
	wormhole
	PUBLIC ${SCREEN} # ? Using in the contructor: ScreenItem*
	PUBLIC ${RECT} # ? Compose RectangleShape* and RectangleCore*
	PUBLIC ${MOD_PENGINE} # ? Compose PhysicsEngine* and use it's features, Compose IMechanics* and use it features
	PUBLIC ${MOD_LOCATOR} # ? Compose Locator* and use it's features
)
target_link_libraries(
	wormhole
	PRIVATE physics_engine
	PRIVATE locator
	PRIVATE rectangle
)
set_specific_definitions(
	wormhole
)
set_output_properties(wormhole)
