# ? Providing linker flags
if(NOT UNIX)
	if(CMAKE_GENERATOR MATCHES "Visual Studio .*")
		STRING(REGEX REPLACE "/SUBSYSTEM:CONSOLE" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
		STRING(REGEX REPLACE "/SUBSYSTEM:WINDOWS" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

		if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE") # ? Show console with debug information
		else()
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") # ? Disable console appearence
		endif()
	else()
		STRING(REGEX REPLACE "-mconsole" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
		STRING(REGEX REPLACE "-mwindows" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

		if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole") # ? Show console with debug information
		else()
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows") # ? Disable console appearence
		endif()
	endif()
endif()
