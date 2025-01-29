include(FetchContent)
set(FETCHCONTENT_QUIET FALSE) # ? To allow cmake print intermidiate comments

# ? Define SDL version and URLs
set(SDL_VERSION "2.30.2") # 2.3.11

# ? Link to source code
set(SDL_URL "https://github.com/libsdl-org/SDL/archive/refs/tags/release-${SDL_VERSION}.zip")

message(STATUS "\tFetching SDL-v.${SDL_VERSION} from ${SDL_URL}...")

FetchContent_Declare(
  SDL2
  URL ${SDL_URL}
)
FetchContent_MakeAvailable(SDL2)

# ? Define SDL_image version and URLs
set(SDL_Image_VERSION "2.8.2") # 2.8.4

# ? Link to source code
set(SDL_Image_URL "https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-${SDL_Image_VERSION}.zip")

message(STATUS "\tFetching SDL_image-v.${SDL_Image_VERSION} from ${SDL_Image_URL}...")

FetchContent_Declare(
  SDL2_image
  URL ${SDL_Image_URL}
)
FetchContent_MakeAvailable(SDL2_image)

# TODO: Some other useful libs for the future projects

# # ? Define SDL_mixer version and URLs
# set(SDL_Mixer_VERSION "2.8.0")

# # ? Link to source code
# set(SDL_Mixer_URL "https://github.com/libsdl-org/SDL_mixer/archive/refs/tags/release-${SDL_Mixer_VERSION}.zip")

# message(STATUS "\tFetching SDL_mixer-v.${SDL_Mixer_VERSION} from ${SDL_Mixer_URL}...")

# # ! Need opusfile, flac, libxmp, fluidsynth, wavpack
# FetchContent_Declare(
  #   SDL2_mixer
  #   URL ${SDL_Mixer_URL}
  # )
# FetchContent_MakeAvailable(SDL2_mixer)

# # ? Define SDL_ttf version and URLs
# set(SDL_TTF_VERSION "2.22.0")

# # ? Link to source code
# set(SDL_TTF_URL "https://github.com/libsdl-org/SDL_ttf/archive/refs/tags/release-${SDL_TTF_VERSION}.zip")

# message(STATUS "\tFetching SDL_ttf-v.${SDL_TTF_VERSION} from ${SDL_TTF_URL}...")

# FetchContent_Declare(
  #   SDL2_ttf
  #   URL ${SDL_TTF_URL}
  # )
  # FetchContent_MakeAvailable(SDL2_ttf)
    
# TODO: Some other useful libs for the future projects
