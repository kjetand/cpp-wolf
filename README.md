# cpp-wolf
The classic _Wolfenstein 3D_ game in `C++20`.

`cpp-wolf` is a fork of `Chocolate-Wolfenstein-3D` which implements
a SDL port of the classic _Wolfenstein 3D_ game (by _Id Software_).
The idea behind the `cpp-wolf` project is to to modernize the existing
code base using modern `C++20`, and to make it easy build it cross-platform
using `cmake`.

## Credits
All credit goes out to:

- _Id software_ (http://www.idsoftware.com) who made this awesome game,
  and open sourced [the code](https://github.com/id-Software/wolf3d).
- _Moritz "Ripper" Kroll_ who made the SDL port named `Wolf4SDL`.
- _Fabien Sanglard_ who made
  [`Chocolate-Wolfenstein-3D`](https://github.com/fabiensanglard/Chocolate-Wolfenstein-3D).

## Build

### Windows
`cpp-wolf` uses `cmake`. In order to build the code, from project root directory:

```
mkdir build
cd build
cmake .. -DSDL_ROOT="/path/to/SDL-1.2.15" -DSDL_MIXER_ROOT="/path/to/SDL_mixer-1.2.12/"
```

When running the game it will probably output something like _"NO WOLFENSTEIN 3D
DATA FILES to be found!"_. This is because you are missing the asset files that
are shipped with the original game. These are not part of this repo due to copyright
reasons. Either download the _shareware version_ of _Wolfenstein 3D_ to obtain the
`.WL1` files, or buy the _full version_ to obtain the full episode `.WL6` files.
Locate these files and either define `-DCPP_WOLF_SHAREWARE_ASSETS_DIR=/path/to/assets`,
or `-DCPP_WOLF_FULL_ASSETS_DIR=/path/to/assets`. The asset files will then
automatically be copied into the binary directory of the build.

### Linux
Currently not supported.

## Project status
Just started on the project, where code currently builds with `MSVC 19.25`
compiler on `Windows 10`.
