# ? We using SDL2 as shared libraries so they should be in the same directory with Game executable
if(WIN32) # ! Why Linux don't need it?
  set(EXTENSION_NAME "dll")
  set(PREFIX "")

  # Define function to copy DLL's on Windows
  function(copy_dll MODULE)
    # set(POSTFIX_DLL_NAME "")
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      set(POSTFIX_DLL_NAME "d") # set to 'd' when Debug
    else()
      set(POSTFIX_DLL_NAME "")
    endif()

    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "$<TARGET_FILE_DIR:${MODULE}::${MODULE}>/${PREFIX}${MODULE}${POSTFIX_DLL_NAME}.${EXTENSION_NAME}"
      $<TARGET_FILE_DIR:${PROJECT_NAME}>/"${PREFIX}${MODULE}${POSTFIX_DLL_NAME}.${EXTENSION_NAME}"
    )
  endfunction()

  copy_dll(SDL2)
  copy_dll(SDL2_image)

  # copy_dll(SDL2_ttf) # TODO: for future releases
  # copy_dll(SDL2_mixer) # TODO: for future releases
endif()
