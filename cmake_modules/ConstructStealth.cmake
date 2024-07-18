# ? Create separate library for the jet pack ability
add_library(
	stealth
	${LI_TYPE}
	${GAME_INCLUDES}/abilities/stealth/Stealth.cpp
)
target_include_directories(
	stealth
	PUBLIC ${GAME_INCLUDES}
)
set_specific_definitions(
	stealth
)
set_output_properties(stealth)
