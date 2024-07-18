# ? Create separate library for the reactangle features
add_library(
	rectangle
	${LIB_TYPE}
	${GAME_INCLUDES}/rectangle/RectangleShape.cpp
	${GAME_INCLUDES}/rectangle/RectangleCore.cpp
)
target_include_directories(
	rectangle
	PUBLIC ${GAME_INCLUDES}
)
set_specific_definitions(
	rectangle
)
set_output_properties(rectangle)
