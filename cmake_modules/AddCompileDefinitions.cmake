message("Build type: " ${CMAKE_BUILD_TYPE})

add_definitions(-D_WINDOWS)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_definitions(-D_DEBUG)
endif()
