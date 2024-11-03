# ? Create separate library for the gun
add_library(
	gun
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/gun/Gun.cpp
)
target_include_directories(
	gun
	PUBLIC ${GAME_INCLUDES}
	PUBLIC ${VC_EXPORT_INCLUDE}
)
target_link_libraries(
	gun
	PUBLIC "${SDL2_Connector_LIBRARY}"
)
set_specific_definitions(
	gun
)
set_output_properties(gun)
