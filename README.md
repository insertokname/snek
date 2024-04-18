# snek
snek is a snake algorithm made in sdl

**General structure**
This project has two branches:
- The algorithm banch
- The game branch

The game branch is the interactive version of this algorithm that you can play with you arrow keys.

The algorithm branch is the version that automatically completes the game on its own.

## Building steps:

If your on **nixos** or have the **nix** package manager installed and you have **flakes** enable you can just pull the repo and run it with `nix run`

**Building on your own:**

**Depencies:**
- SDL2
- make
- gcc

**For example on ubuntu:**

install sdl2:
`sudo apt install libsdl2-dev`
install make:
`sudo apt install make`
install make:
`sudo apt install gcc`

for other distros checkout [this guide](https://wiki.libsdl.org/SDL2/Installation)