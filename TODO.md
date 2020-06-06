# TODO
- [ ] `statetype.rotate` should not be a `bool`. Values range `0-2`.
      Probably, a bug is introduced by refactoring. The compiler warnings
      about `bool` conversion is a good indication of where the bug is
      introduced.
- [ ] Clean up `wl_def.h`. Contains a lot of macros and enums used throughout
      the whole code base. E.g. use `constexpr` instead of macros.
- [ ] Apply `clang-tidy` modernize fixes.