# ? Create separate library for the locator module
add_library(
	locator
	${LIB_TYPE}
	${MOD_LOCATOR}/Locator.cpp
	${MOD_LOCATOR}/Locator.hpp
)
target_include_directories(
	locator
	PUBLIC ${RECT} # ? Use features from RectangleCore
	PUBLIC ${MOD_PENGINE} # ? Interface
)
target_link_libraries(
	locator
	PRIVATE rectangle
)
set_specific_definitions(
	locator
)
set_output_properties(locator)
