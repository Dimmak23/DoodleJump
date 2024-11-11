# ? Define SDL version and URLs
set(SDL_VERSION "2.30.1")

# ? Link to source code
set(SDL_URL "https://github.com/libsdl-org/SDL/releases/download/release-${SDL_VERSION}/SDL2-${SDL_VERSION}.zip")

message(STATUS "\tDownloading SDL-v.${SDL_VERSION} from ${SDL_URL}...")

# ? Configure the ExternalProject
ExternalProject_Add(
	SDL
	URL ${SDL_URL}
	PREFIX ${CMAKE_BINARY_DIR}/SDL

	# ? This is will install include, dll's and lib's to appropriate folder
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${SDL_INSTALL_DIR}
)
