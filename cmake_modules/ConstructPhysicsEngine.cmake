# ? Create separate library for the physics engine module
add_library(
	physics_engine
	${LIB_TYPE}
	${GAME_INCLUDES}/modules/physics_engine/PhysicsEngine.cpp
)
target_include_directories(
	physics_engine
	PUBLIC ${GAME_INCLUDES}
)
target_link_libraries(
	physics_engine
	PRIVATE rectangle
)
set_specific_definitions(
	physics_engine
)
set_output_properties(physics_engine)
