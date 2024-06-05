# ? Create separate library for the score board
add_library(
	score_board
	${LIB_TYPE}
	${SCR_BOARD}/ScoreBoard.cpp
	${SCR_BOARD}/ScoreBoard.hpp
)
target_include_directories(
	score_board
	PUBLIC ${SCREEN} # ? Composes with ScreenItem*
	PUBLIC ${UNI_FEATURES} # ? Composes with IAccounting* and using it's features
	PUBLIC ${WR_SPRITE} # ? Composes with SpriteWrapper*
	PUBLIC ${MOD_LOCATOR} # ? Inherits IRelocatableActor*, composes with Locator*
	PUBLIC ${WR_ANISPRITE} # ? Composes with AnimatedSpriteWrapper*
	PUBLIC ${RECT} # ? Composes with RectangleCore* and uses it's features
	PUBLIC ${UTILS}
)
target_link_libraries(
	score_board
	PRIVATE sprite_wrapper
	PRIVATE locator
	PRIVATE animated_character
	PRIVATE rectangle
)
set_specific_definitions(
	score_board
)
set_output_properties(score_board)
