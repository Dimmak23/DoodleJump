# ? Create separate library for the physics engine module
add_library(
	physics_engine
	${LIB_TYPE}
	${MOD_PENGINE}/PhysicsEngine.cpp
	${MOD_PENGINE}/PhysicsEngine.hpp
)
target_include_directories(
	physics_engine
	PUBLIC ${RECT} # ? Use features from RectangleCore
	PUBLIC ${UTILS}
)
target_link_libraries(
	physics_engine
	PRIVATE rectangle
)
set_specific_definitions(
	physics_engine
)
set_output_properties(physics_engine)
