#pragma once

#include "SDL_render.h"
#include "dimensions.hpp"

namespace snek {
    class Context {
    public:
        SDL_Renderer *renderer = nullptr;
        SDL_Window *window = nullptr;
        explicit Context(const Dimensions &window_size);
        ~Context();
    };
}