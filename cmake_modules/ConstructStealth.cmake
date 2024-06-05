# ? Create separate library for the jet pack ability
add_library(
	stealth
	${LI_TYPE}
	${ABL_STEALTH}/Stealth.cpp
	${ABL_STEALTH}/Stealth.hpp
)
target_include_directories(
	stealth
	PUBLIC ${WR_FEATURES} # ? Compose with IGraphicle*
)
set_specific_definitions(
	stealth
)
set_output_properties(stealth)
