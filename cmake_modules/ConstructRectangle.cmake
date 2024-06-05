# ? Create separate library for the reactangle features
add_library(
	rectangle
	${LIB_TYPE}
	${RECT}/RectangleShape.cpp
	${RECT}/RectangleShape.hpp
	${RECT}/RectangleCore.cpp
	${RECT}/RectangleCore.hpp
)
target_include_directories(
	rectangle
	PUBLIC ${SCREEN} # ? Composes with ScreenItem*
	PUBLIC ${UTILS}
)
set_specific_definitions(
	rectangle
)
set_output_properties(rectangle)
