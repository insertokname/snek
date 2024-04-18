#include "loop.hpp"

namespace snek {
    void prepareScene(snek::App *app) {
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
    }

    void presentScene(snek::App *app) {
        SDL_RenderPresent(app->renderer);
    }

    void doInput(std::pair<int, int> &direction) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit(0);
                    break;

                case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_UP) {
                        direction = std::pair<int, int>(-1, 0);
                    }
                    else if (event.key.keysym.sym == SDLK_DOWN) {
                        direction = std::pair<int, int>(1, 0);
                    }
                    else if (event.key.keysym.sym == SDLK_LEFT) {
                        direction = std::pair<int, int>(0, -1);
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT) {
                        direction = std::pair<int, int>(0, 1);
                    }
                }
                default:
                    break;
            }
        }
    }
}