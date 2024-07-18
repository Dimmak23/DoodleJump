# ? Create separate library for the ammo projectile actor
add_library(
	ammo
	${LIB_TYPE}
	${GAME_INCLUDES}/projectiles/ammo/Ammo.cpp
)
target_include_directories(
	ammo
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	ammo
	PRIVATE image
	PRIVATE locator
	PRIVATE physics_engine
)
set_specific_definitions(
	ammo
)
set_output_properties(ammo)
