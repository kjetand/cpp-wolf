cmake_minimum_required(VERSION 3.15)
project(cpp-wolf CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)

function(copy_files dir ext)
  file(GLOB assets ${dir}/*.${ext})

  foreach (asset ${assets})
    message("-- Copy ${ext} ${asset} to ${PROJECT_BINARY_DIR}/bin")
    file(COPY ${asset} DESTINATION ${PROJECT_BINARY_DIR}/bin)
  endforeach ()
endfunction()

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
  set(SDL_MIXER_INCLUDE_DIR ${SDL_MIXER_ROOT}/include)
  set(SDL_LIBRARY ${SDL_ROOT}/lib/x86/SDL.lib)
  set(SDL_MIXER_LIBRARY ${SDL_MIXER_ROOT}/lib/x86/SDL_mixer.lib)

  copy_files(${SDL_ROOT}/lib/x86 "dll")
  copy_files(${SDL_MIXER_ROOT}/lib/x86 "dll")
endif ()

if (CPP_WOLF_SHAREWARE_ASSETS_DIR)
  copy_files(${CPP_WOLF_SHAREWARE_ASSETS_DIR} "WL1")
  add_definitions(-DUPLOAD -DGOODTIMES -DCARMACIZED)
elseif (CPP_WOLF_FULL_ASSETS_DIR)
  copy_files(${CPP_WOLF_FULL_ASSETS_DIR} "WL6")
  add_definitions(-DGOODTIMES -DCARMACIZED)
else ()
  add_definitions(-DGOODTIMES -DCARMACIZED)
  message(WARNING "In order to play the 'Wolfenstein 3D' game, the runtime requires asset files that contains art, "
      "sound, etc. These are not included by this repo due to copyright reasons. Either download the shareware "
      "version of 'Wolfenstein 3D' to obtain the .WL1 files, or buy the full version to obtain the full episode .WL6 "
      "files. Locate these files and either define '-DCPP_WOLF_SHAREWARE_ASSETS_DIR=/path/to/assets', or "
      "'-DCPP_WOLF_FULL_ASSETS_DIR=/path/to/assets'. The asset files will then automatically be copied into the "
      "binary directory of the build.")
endif ()

add_subdirectory(src)
