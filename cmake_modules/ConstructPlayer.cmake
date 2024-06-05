# ? Create separate library for the player actor
add_library(
	player
	${LIB_TYPE}
	${ACT_PLAYER}/Player.cpp
	${ACT_PLAYER}/Player.hpp
)
target_include_directories(
	player
	PUBLIC ${ACT_FEATURES} # ? Inherit from IShooter*
	PUBLIC ${SCREEN} # ? Composes with ScreenItem*
	PUBLIC ${WR_ANISPRITE} # ? Compose AnimatedSpriteWrapper*
	PUBLIC ${MOD_LOCATOR} # ? Compose Locator*
	PUBLIC ${MOD_PENGINE} # ? Compose PhysicsEngine*
	PUBLIC ${UTILS} # ? Path, Sizes
	PUBLIC ${WR_FEATURES}
)
target_link_libraries(
	player
	PRIVATE animated_character
	PRIVATE locator
	PRIVATE physics_engine
	PRIVATE rectangle
)
set_specific_definitions(
	player
)
set_output_properties(player)
