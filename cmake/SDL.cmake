include(Functions)

if (MSVC)
  if (NOT SDL_ROOT)
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/SDL-1.2.15)
      message("-- Downloading SDL")
      file(DOWNLOAD https://www.libsdl.org/release/SDL-devel-1.2.15-VC.zip ${CMAKE_BINARY_DIR}/SDL.zip
          TIMEOUT 60
          SHOW_PROGRESS)
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/SDL.zip
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif ()
    set(SDL_ROOT ${CMAKE_BINARY_DIR}/SDL-1.2.15)
  endif ()

  if (NOT SDL_MIXER_ROOT)
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/SDL_mixer-1.2.12)
      message("-- Downloading SDL mixer")
      file(DOWNLOAD https://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-devel-1.2.12-VC.zip ${CMAKE_BINARY_DIR}/SDL_mixer.zip
          TIMEOUT 60
          SHOW_PROGRESS)
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/SDL_mixer.zip
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif ()
    set(SDL_MIXER_ROOT ${CMAKE_BINARY_DIR}/SDL_mixer-1.2.12)
  endif ()

  set(SDL_INCLUDE_DIR ${SDL_ROOT}/include)
  set(SDLMIXER_INCLUDE_DIR ${SDL_MIXER_ROOT}/include)
  set(SDL_LIBRARY ${SDL_ROOT}/lib/x86/SDL.lib)
  set(SDLMIXER_LIBRARY ${SDL_MIXER_ROOT}/lib/x86/SDL_mixer.lib)

  copy_files("dll" ${SDL_ROOT}/lib/x86 ${CPP_WOLF_BINARY_DIR})
  copy_files("dll" ${SDL_MIXER_ROOT}/lib/x86 ${CPP_WOLF_BINARY_DIR})
else ()
  find_package(SDL REQUIRED)
  find_package(SDL_mixer REQUIRED)
endif ()
