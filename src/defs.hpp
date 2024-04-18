#pragma once

#include "SDL2/SDL.h"
namespace snek {
    constexpr std::size_t SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720
        , CELL_SIZE = 50, BOARD_HEIGHT = 3, BOARD_WIDTH = 6;

    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
    };
}