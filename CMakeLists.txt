cmake_minimum_required(VERSION 3.15)
project(cpp-wolf CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

if (MSVC)
  if (NOT SDL_ROOT)
    message(FATAL_ERROR "${PROJECT_NAME} for Windows requires SDL 1.2.15. Please download the specified version from "
        "e.g. https://www.libsdl.org/release/SDL-devel-1.2.15-VC.zip, and specify '-DSDL_ROOT=<path-to-SDL>' in your "
        "cmake command.")
  endif ()
  if (NOT SDL_MIXER_ROOT)
    message(FATAL_ERROR "${PROJECT_NAME} for Windows requires SDL_mixer 1.2.12. Please download the specified version "
        "from e.g. https://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-devel-1.2.12-VC.zip, and specify "
        "'-DSDL_MIXER_ROOT=<path-to-SDL_mixer>' in your cmake command.")
  endif ()

  set(SDL_INCLUDE_DIR ${SDL_ROOT}/include)
  set(SDL_MIXER_INCLUDE_DIR ${SDL_MIXER_ROOT}/include)
  set(SDL_LIBRARY ${SDL_ROOT}/lib/x86/SDL.lib)
  set(SDL_MIXER_LIBRARY ${SDL_MIXER_ROOT}/lib/x86/SDL_mixer.lib)

  file(GLOB_RECURSE sdl_dlls ${SDL_ROOT}/lib/x86/*.dll)
  file(GLOB_RECURSE sdl_mixer_dlls ${SDL_MIXER_ROOT}/lib/x86/*.dll)

  foreach (dll ${sdl_dlls})
    message("-- Copy DLL ${dll} to ${CMAKE_BINARY_DIR}/src")
    file(COPY ${dll} DESTINATION ${CMAKE_BINARY_DIR}/src)
  endforeach ()

  foreach (dll ${sdl_mixer_dlls})
    message("-- Copy DLL ${dll} to ${CMAKE_BINARY_DIR}/src")
    file(COPY ${dll} DESTINATION ${CMAKE_BINARY_DIR}/src)
  endforeach ()
endif ()

add_subdirectory(src)
