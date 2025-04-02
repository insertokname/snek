#include "context.hpp"

#include <iostream>
#include <ostream>

#include "SDL.h"
#include "SDL_ttf.h"
#include "dimensions.hpp"

namespace snek {
    Context::Context(const Dimensions& window_size) {
        int renderer_flags = SDL_RENDERER_ACCELERATED;
        int window_flags = SDL_WINDOW_RESIZABLE;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "Couldn't initialize SDL: " << SDL_GetError() << "\n";
            exit(1);
        }

        if (TTF_Init() < 0) {
            std::cout << "Couldn't initialize SDL_ttf: " << TTF_GetError()
                      << "\n";
            exit(1);
        }

        this->window = SDL_CreateWindow("snek",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        static_cast<int>(window_size.width),
                                        static_cast<int>(window_size.height),
                                        window_flags);

        if (this->window == nullptr) {
            std::cout << "Failed to open " << window_size.width << " x "
                      << window_size.height << " window: " << SDL_GetError()
                      << "\n";
            exit(1);
        }

        SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        this->renderer = SDL_CreateRenderer(this->window, -1, renderer_flags);

        if (this->renderer == nullptr) {
            std::cout << "Failed to create renderer: " << SDL_GetError()
                      << "\n";
            exit(1);
        }
    }

    Context::~Context() {
        TTF_Quit();
        SDL_Quit();
    }
}