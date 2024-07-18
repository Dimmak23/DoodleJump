# ? Create separate library for the gun
add_library(
	gun
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/gun/Gun.cpp
)
target_include_directories(
	gun
	PUBLIC ${GAME_INCLUDES}
)
set_specific_definitions(
	gun
)
set_output_properties(gun)
