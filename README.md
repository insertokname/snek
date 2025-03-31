# snek
snek is a snake algorithm made in sdl that automatically solves every board that has at least one even side

**General structure**
This project has two branches:
- The algorithm banch
- The game branch

### Branches:
- The game branch is the interactive version of this algorithm that you can play with you arrow keys.
- The algorithm branch is the version that automatically completes the game on its own.

# Native developement:

### Dependencies:

**macos or linux with nix package manager**
- run `nix develop` or directly run or build with `nix run` or `nix build` 

**macos no nix**
- [xcode tools](https://mac.install.guide/commandlinetools/3)
- cmake (`brew install cmake`)
- sdl2 (`brew install sdl2`)
**game "OS"**:
- [visual studio](https://visualstudio.microsoft.com/downloads/)
- cmake can be downloaded with scoop or choco or from [here](https://cmake.org/download/)
- download **SDL2** (the version `SDL2-devel-2.30.1-VC.zip) from [here](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.1)
- also download SDL2_ttf (the version `
SDL2_ttf-devel-2.24.0-VC.zip`) from [here](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0)
- set the SDL2DIR to the extracted location of the sdl 2 directory
- also set SDL2TTFDIR to the extracted location of the sdl 2 ttf diretory
- setup CMAKE_PREFIX_PATH environment variable to point to the directory where sdl2 was extracted
- stept are very well outlined in this [tutorial](https://youtu.be/jkmDBZzchbQ?feature=shared)


### Building:

Building is the same for all platforms (yes even for gaming os), you can simply run the following:

```
mkdir build
cd build
cmake ..
cmake --build .
```

# Wasm development:

### Dependencies:

**if you use nix**:
- run `nix develop .#wasm`
**any other platform**
- all of the native dependencies mentioned above
- [emscripten](https://emscripten.org/)

### Building:

```
mkdir build_wasm
cd build_wasm
emcmake cmake ..
cmake --build .
```