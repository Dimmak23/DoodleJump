# ? Need to find SDL2 libraries to link to SDL2_image
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(SDL2_LIBRARY "${SDL_BINARY}/SDL2.dll")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(SDL2_LIBRARY "${SDL_LIB}/SDL2")

	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set(SDL2_LIBRARY "${SDL2_LIBRARY}d.lib")
	else()
		set(SDL2_LIBRARY "${SDL2_LIBRARY}.lib")
	endif()
endif()

# ? Also need SDL2_main
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(SDL2_MAIN_LIBRARY "${SDL_LIB}/libSDL2main.a")
endif()

# ? Define SDL_image version and URLs
set(SDL_IMAGE_VERSION "2.8.2")

# ? Link to source code
set(SDL_IMAGE_URL "https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-${SDL_IMAGE_VERSION}.zip")

message(STATUS "\tDownloading SDL_image-v.${SDL_IMAGE_VERSION} from ${SDL_IMAGE_URL}...")

# ? Configure the ExternalProject
if(SDL_IMAGE_NOT_DEPEND_ON_SDL)
	ExternalProject_Add(
		SDL_image
		URL ${SDL_IMAGE_URL}
		PREFIX ${CMAKE_BINARY_DIR}/SDL_image

		# ? This is will install include, dll's and lib's to appropriate folder
		CMAKE_ARGS
		-DSDL2_LIBRARY=${SDL2_LIBRARY}
		-DSDL2_MAIN_LIBRARY=${SDL2_MAIN_LIBRARY}
		-DSDL2_INCLUDE_DIR=${SDL_INCLUDE}
		-DCMAKE_INSTALL_PREFIX=${SDL_IMAGE_INSTALL_DIR}
		-DSDL2IMAGE_SAMPLES=OFF # ! throw not needed errors, maybe usefull
	)
else()
	ExternalProject_Add(
		SDL_image
		URL ${SDL_IMAGE_URL}
		PREFIX ${CMAKE_BINARY_DIR}/SDL_image
		DEPENDS SDL # ! Specify dependency on SDL

		# ? This is will install include, dll's and lib's to appropriate folder
		CMAKE_ARGS
		-DSDL2_LIBRARY=${SDL2_LIBRARY}
		-DSDL2_MAIN_LIBRARY=${SDL2_MAIN_LIBRARY}
		-DSDL2_INCLUDE_DIR=${SDL_INCLUDE}
		-DCMAKE_INSTALL_PREFIX=${SDL_IMAGE_INSTALL_DIR}
		-DSDL2IMAGE_SAMPLES=OFF # ! throw not needed errors, maybe usefull
	)
endif()
