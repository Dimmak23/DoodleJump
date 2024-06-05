# ? Create separate library for the collision module
add_library(
	collider
	${LIB_TYPE}
	${MOD_COLLIDER}/Collider.cpp
	${MOD_COLLIDER}/Collider.hpp
)
target_include_directories(
	collider
	PUBLIC ${MOD_PENGINE} # ? Interface
	PUBLIC ${RECT} # ? Use features from RectangleCore
	PUBLIC ${UTILS}
	PUBLIC ${LVL_MAIN} # ? Interface
	PUBLIC ${SCREEN} # ? Not used but Level.hpp inserts ScreenItem.hpp
	PUBLIC ${UNI_FEATURES} # ? Compose with IAccounting* and using features
)
target_link_libraries(
	collider
	PRIVATE rectangle
)
set_specific_definitions(
	collider
)
set_output_properties(collider)
