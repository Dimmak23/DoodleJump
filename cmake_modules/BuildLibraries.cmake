# ? ${LIB_TYPE} - Which type of libraries is it gonna be?
# ? Such variable setted in the cmake_config_BUILD_TYPE.ps1

# ? Grab all modules that start with "Construct..." in their names
# ? it's the library construction modules
file(GLOB CUSTOM_LIBRARIES "${CMAKE_MODS}/Construct*.cmake")

foreach(MODULE ${CUSTOM_LIBRARIES})
	# ? Include each
	include(${MODULE})
endforeach()
