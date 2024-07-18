# ? Create separate library for the locator module
add_library(
	locator
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/locator/Locator.cpp
)
target_include_directories(
	locator
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	locator
	PRIVATE rectangle
)
set_specific_definitions(
	locator
)
set_output_properties(locator)
