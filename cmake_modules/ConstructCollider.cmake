# ? Create separate library for the collision module
add_library(
	collider
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/collider/Collider.cpp
)
target_include_directories(
	collider
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	collider
	PRIVATE rectangle
)
set_specific_definitions(
	collider
)
set_output_properties(collider)
