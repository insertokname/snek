#pragma once

#include "SDL_render.h"
#include "dimensions.hpp"

namespace snek {
    class VideoContext {
    public:
        SDL_Renderer *renderer = nullptr;
        SDL_Window *window = nullptr;
        explicit VideoContext(const Dimensions &window_size);
        ~VideoContext();
    };
}