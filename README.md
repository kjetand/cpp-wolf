```text
 ██████ ██████  ██████        ██     ██  ██████  ██      ███████ 
██      ██   ██ ██   ██       ██     ██ ██    ██ ██      ██      
██      ██████  ██████  █████ ██  █  ██ ██    ██ ██      █████   
██      ██      ██            ██ ███ ██ ██    ██ ██      ██      
 ██████ ██      ██             ███ ███   ██████  ███████ ██
```

The `cpp-wolf` project aims to modernize the existing code base of
`Chocolate-Wolfenstein-3D` using modern `C++20` and `cmake`.

|Compiler|Status|
|--------|------|
|MSVC|[![AppVeyor Build status](https://img.shields.io/appveyor/ci/kjetand/cpp-wolf.svg)](https://ci.appveyor.com/project/kjetand/cpp-wolf)|
|GCC/Clang|[![Build Status](https://travis-ci.org/kjetand/cpp-wolf.svg?branch=master)](https://travis-ci.org/kjetand/cpp-wolf)|

## Credits
All credit goes out to:

- [_Id software_](http://www.idsoftware.com) who made this awesome game,
  and open sourced [the code](https://github.com/id-Software/wolf3d).
- _Moritz "Ripper" Kroll_, who made the SDL port named `Wolf4SDL`.
- _Fabien Sanglard_, the implementor of
  [`Chocolate-Wolfenstein-3D`](https://github.com/fabiensanglard/Chocolate-Wolfenstein-3D).
- [_Jason "lefticus" Turner_](https://github.com/lefticus), for inspiration on his
  `Doom` port to `C++20`.

## Build

### Windows
`cpp-wolf` uses `cmake` and must be compiled with a `x86` compiler. E.g.

```
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A win32 -DCMAKE_BUILD_TYPE:STRING=Release
cmake --build . --config "Release"
```

The above commands will download dependencies automatically for you. However, if you
want to use local dependencies, you can define their paths explicitly with `SDL_ROOT`
and `SDL_MIXER_ROOT`.

When running the game it will probably output something like _"NO WOLFENSTEIN 3D
DATA FILES to be found!"_. This is because you are missing the asset files that
are shipped with the original game. These are not part of this repo due to copyright
reasons. Either download the _shareware version_ of _Wolfenstein 3D_ to obtain the
`.WL1` files, or buy the _full version_ to obtain the full episode `.WL6` files.
Define `-DCPP_WOLF_SHAREWARE_ASSETS_DIR=/path/to/assets` or
`-DCPP_WOLF_FULL_ASSETS_DIR=/path/to/assets`, depending on your version of the game.
The `WL`-files will be copied for you automatically.

### Linux
Currently not supported.

## Project status
Just started on the project, where code currently builds with `MSVC 19.25`
compiler on `Windows 10`.
