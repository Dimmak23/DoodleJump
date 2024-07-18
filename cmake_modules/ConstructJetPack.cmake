# ? Create separate library for the jet pack ability
add_library(
	jet_pack
	${LIB_TYPE}
	${GAME_INCLUDES}/abilities/jet_pack/JetPack.cpp
)
target_include_directories(
	jet_pack
	PUBLIC ${GAME_INCLUDES}
)
set_specific_definitions(
	jet_pack
)
set_output_properties(jet_pack)
