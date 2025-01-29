# TODO: do for other platforms (Linux at least)
if(WIN32)
  # ? SDL2_mixer would need opusfile
  set(OpusFile_LIBRARY "${DEPS}/preinstalled/windows/opusfile/bin")
  set(OpusFile_INCLUDE_PATH "${DEPS}/preinstalled/windows/opusfile/include")

  # set(FLAC_LIBRARY "${DEPS}/preinstalled/windows/flac/bin")
  # set(FLAC_INCLUDE_PATH "${DEPS}/preinstalled/windows/flac/include")

  # ? SDL2_mixer would need opusfile
  set(libxmp_LIBRARY "${DEPS}/preinstalled/windows/libxmp/bin")
  set(libxmp_INCLUDE_PATH "${DEPS}/preinstalled/windows/libxmp/include")

  # ? SDL2_mixer would need fluidsynth
  set(FluidSynth_LIBRARY "${DEPS}/preinstalled/windows/fluidsynth/bin")
  set(FluidSynth_INCLUDE_PATH "${DEPS}/preinstalled/windows/fluidsynth/include")

  # ? SDL2_mixer would need wavpack
  set(wavpack_LIBRARY "${DEPS}/preinstalled/windows/wavpack/bin")
  set(wavpack_INCLUDE_PATH "${DEPS}/preinstalled/windows/wavpack/include")

  # ? Explicitly and globaly include third-party source codes
  include_directories(
    PRIVATE "${DEPS}/preinstalled/windows/ogg/include"
    PRIVATE "${DEPS}/preinstalled/windows/opus/include"
  )
endif()
