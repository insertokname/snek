#pragma once

#include "SDL2/SDL.h"
namespace snek
{
    constexpr int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;
    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
    };
} 