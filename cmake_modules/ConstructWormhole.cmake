# ? Create separate library for the jet pack ability
add_library(
	wormhole
	${LIB_TYPE}
	${GAME_INCLUDES}/abilities/wormhole/Wormhole.cpp
)
target_include_directories(
	wormhole
	PUBLIC ${GAME_INCLUDES}
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
