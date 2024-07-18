# ? Create separate library for the game application
add_library(
	application
	${LIB_TYPE}
	${GAME_INCLUDES}/application/GameApplication.cpp
)
target_include_directories(
	application
	PUBLIC ${VC_EXPORT_INCLUDE}
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	application
	PUBLIC "${SDL2_Connector_LIBRARY}"
	PRIVATE level
)
set_specific_definitions(
	application
)
set_output_properties(application)
