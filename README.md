# snek
snek is a snake algorithm made in sdl2 that automatically solves every board that has at least one even side (the number of cells on the side is even). This project uses **emscripten** and **github actions** to automatically deploy to github pages here: https://insertokname.github.io/snek/algorithm/snek.html and you can also play it in the browser here: https://insertokname.github.io/snek/game/snek.html 

The game can also be played on mobile at the same urls by swiping your finger!

**General structure**
This project is made using cmake and uses `#defines` for conditional compilation. If `SNEK_ALGORITHM` is defined then the game will play itself automatically. If it isn't defined then the player can controll it. The project is setup so it can be build on any os and can also target wasm.

### Quick setup:

- if you just want to download everyhing all at once and don't really care about first build time and such you can try configuring you project using the `AUTO_DOWNLOAD_DEPS ON` option. This will automatically clone the repos locally, build what is needed and then link them. This is done using the cmake `FetchContent_Declare` function inside the build system.
- to achieve this you have to either manually do `set(AUTO_DOWNLOAD_DEPS ON)` inside the root `CMakeLists.txt` file OR when configuring you can do the following:
```
git clone https://github.com/insertokname/snek.git
cd snek
mkdir build
cd build
cmake .. -DAUTO_DOWNLOAD_DEPS=ON
```
this will automatically download all required files.

after this you can normally build your project or let cmake build it with:
```
cmake --build . --parallel
```

### Dependencies (manual download):

- sdl2 and sdl2_ttf
- SDL2_DIR and SDL2TTF_DIR set to the install directories
- install steps are outlined in the next section

### Getting the dependencies:

**macos or linux with nix package manager**
- run `nix develop .#game` or directly run with `nix run .#game` or `nix run .#algorithm` 

**ubuntu**
```
sudo apt update
sudo apt install cmake libsdl2-dev libsdl2-ttf-dev -y
```

**macos no nix**
- [xcode tools](https://mac.install.guide/commandlinetools/3)
- cmake (`brew install cmake`)
- **sdl2** (`brew install sdl2`)
- **sdl2_ttf** (`brew install sdl2_ttf`)
- set the SDL2_DIR to the install location of the sdl 2 directory (for example you can do `brew info sdl2`. The output should contain a location that looks something like this: `/opt/homebrew/Cellar/sdl2/2.32.2` with your install path you should run `export SDL2_DIR="<YOUR_SDL2_INSTALL_PATH>"` replacing `<YOUR_SDL2_INSTALL_PATH>` with your actual path)
- do the same thing for SDL2TTF_DIR (use `brew info sdl2_ttf`)

**gaming "OS"**:
- [visual studio](https://visualstudio.microsoft.com/downloads/)
- cmake can be downloaded with scoop or choco or from [here](https://cmake.org/download/) or from their official site
- download **SDL2** (the version `SDL2-devel-2.30.1-VC.zip) from [here](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.1)
- also download **SDL2_ttf** (the version `
SDL2_ttf-devel-2.24.0-VC.zip`) from [here](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0)
- extract both packages into a folder (i made a new folder: `C:\sdks`)
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

Building is the same for all platforms you can simply run the following:

```
git clone https://github.com/insertokname/snek.git
cd snek
mkdir build
cd build
cmake ..
cmake --build .
```

# Wasm:

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
