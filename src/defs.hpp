#pragma once

#include "SDL2/SDL.h"
namespace snek {
    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
    };

    enum Cell { empty, tail, body, head, food };
}