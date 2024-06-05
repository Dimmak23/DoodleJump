# ? Create separate library for the gun
add_library(
	gun
	${LIB_TYPE}
	${MOD_GUN}/Gun.cpp
	${MOD_GUN}/Gun.hpp
)
target_include_directories(
	gun
	PUBLIC ${UNI_FEATURES} # ? Compose IDynamicSpawn* and use it features
	PUBLIC ${ACT_FEATURES} # ? Compose IShooter* and use it features
	PUBLIC ${RECT} # ? define Point type
	PUBLIC ${ACT_FEATURES} # ? Inherit from IShooter*
)
set_specific_definitions(
	gun
)
set_output_properties(gun)
