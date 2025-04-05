# snek
snek is a snake algorithm made in sdl that automatically solves every board that has at least one even side

**General structure**
This project is made using cmake and uses #defines for conditional compilation. If SNEK_ALGORITHM is defined then the game will play itself. If it isn't then the player can controll it. The project is setup so it can be build on any os and also wasm.

### Dependencies:

- sdl2 and sdl2_ttf
- SDL2_DIR and SDL2TTF_DIR set to the install directories
- install steps are outlined in the next section

### Getting the dependencies:

**macos or linux with nix package manager**
- run `nix develop` or directly run or build with `nix run` or `nix build` 

**macos no nix**
- [xcode tools](https://mac.install.guide/commandlinetools/3)
- cmake (`brew install cmake`)
- **sdl2** (`brew install sdl2`)
- **sdl2_ttf** (`brew install sdl2_ttf`)
- set the SDL2_DIR to the install location of the sdl 2 directory (for example you can do `brew info sdl2`. The output should contain a location that looks something like this: `/opt/homebrew/Cellar/sdl2/2.32.2` with your install path you should run `export SDL2_DIR="<YOUR_SDL2_INSTALL_PATH>"` replacing `<YOUR_SDL2_INSTALL_PATH>` with your actual path)
- do the same thing for SDL2TTF_DIR (use `brew info sdl2_ttf`)

**gaming "OS"**:
- [visual studio](https://visualstudio.microsoft.com/downloads/)
- cmake can be downloaded with scoop or choco or from [here](https://cmake.org/download/)
- download **SDL2** (the version `SDL2-devel-2.30.1-VC.zip) from [here](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.1)
- also download **SDL2_ttf** (the version `
SDL2_ttf-devel-2.24.0-VC.zip`) from [here](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0)
- set the `SDL2_DIR` to the extracted location of the sdl 2 directory
- also set `SDL2TTF_DIR` to the extracted location of the sdl 2 ttf diretory
- setup `CMAKE_PREFIX_PATH` environment variable to point to a parent directory of both the extraction location of sdl2 and sdl2_ttf

this is how my setup looks:
```
C:\SDKS
├───SDL2
│   ├───cmake
│   ├───docs
│   ├───include
│   └───lib
│       ├───x64
│       └───x86
└───SDL2_ttf
    ├───cmake
    ├───include
    └───lib
        ├───x64
        └───x86
```

and the env variables set like this:

```
CMAKE_PREFIX_PATH:  C:\sdks
SDL2_DIR:           C:\sdks\SDL2
SDL2TTF_DIR         C:\sdks\SDL2_ttf
```

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