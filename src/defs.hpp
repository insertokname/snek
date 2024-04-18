#pragma once

#include "SDL2/SDL.h"
namespace snek
{
    constexpr std::size_t SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720
        , CELL_SIZE = 20, BOARD_HEIGHT =40, BOARD_WIDTH=40;

    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
    };
} 