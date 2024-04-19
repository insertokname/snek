#pragma once

#include "SDL2/SDL.h"
namespace snek {
    constexpr std::size_t SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720
        , CELL_SIZE = 40, BOARD_HEIGHT = 10, BOARD_WIDTH = 20
        , MOVE_SPEED = 100000;
    //1000000 = 1 sec

    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
    };
}