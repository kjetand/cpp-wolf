include(Functions)

set(asset_extension "WL6")

if (CPP_WOLF_SHAREWARE)
  message("-- Generating 'Wolfenstein 3D' shareware build")
  set(asset_extension "WL1")
  add_definitions(-DUPLOAD -DGOODTIMES -DCARMACIZED)
else ()
  message("-- Generating 'Wolfenstein 3D' full build")
  add_definitions(-DGOODTIMES -DCARMACIZED)
endif ()

if (CPP_WOLF_ASSETS_DIR)
  copy_files(${asset_extension} ${CPP_WOLF_ASSETS_DIR} ${CPP_WOLF_BINARY_DIR})
else ()
  message(WARNING "In order to play the 'Wolfenstein 3D' game, the runtime requires asset files that contains art, "
      "sound, etc. These are not included by this repo due to copyright reasons. Either download the shareware "
      "version of 'Wolfenstein 3D' to obtain the .WL1 files, or buy the full version to obtain the full episode .WL6 "
      "files. Locate these files and define '-DCPP_WOLF_ASSETS_DIR=/path/to/assets'. The full version is by default. "
      "To enable 'shareware' version, define '-DCPP_WOLF_SHAREWARE=TRUE'.")
endif ()
