# ? Create separate library for the jet pack ability
add_library(
	jet_pack
	${LIB_TYPE}
	${ABL_JETPACK}/JetPack.cpp
	${ABL_JETPACK}/JetPack.hpp
)
target_include_directories(
	jet_pack
	PUBLIC ${MOD_PENGINE} # ? Compose IMechanics* and use it features
)
set_specific_definitions(
	jet_pack
)
set_output_properties(jet_pack)
