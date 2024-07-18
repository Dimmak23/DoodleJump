# ? Create separate library for the score board
add_library(
	score_board
	${LIB_TYPE}
	${GAME_INCLUDES}/score_board/ScoreBoard.cpp
)
target_include_directories(
	score_board
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	score_board
	PRIVATE image
	PRIVATE locator
	PRIVATE animated_image
	PRIVATE rectangle
)
set_specific_definitions(
	score_board
)
set_output_properties(score_board)
