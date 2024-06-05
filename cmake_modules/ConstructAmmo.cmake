# ? Create separate library for the ammo projectile actor
add_library(
	ammo
	${LIB_TYPE}
	${PT_AMMO}/Ammo.cpp
	${PT_AMMO}/Ammo.hpp
)
target_include_directories(
	ammo
	PUBLIC ${MOD_LOCATOR} # ? Inherit from IRelocatableActor to impliment relocate
	PUBLIC ${SCREEN} # ? Composes with ScreenItem*
	PUBLIC ${WR_SPRITE} # ? Compose SpriteWrapper*
	PUBLIC ${MOD_LOCATOR} # ? Compose Locator*
	PUBLIC ${MOD_PENGINE} # ? Compose PhysicsEngine*
	PUBLIC ${UTILS} # ? Path, Sizes
)
target_link_libraries(
	ammo
	PRIVATE sprite_wrapper
	PRIVATE locator
	PRIVATE physics_engine
)
set_specific_definitions(
	ammo
)
set_output_properties(ammo)
