#include "loop.hpp"

namespace snek {
    void prepareScene(snek::App *app) {
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
    }

    void presentScene(snek::App *app) {
        SDL_RenderPresent(app->renderer);
    }

    void doInput(void) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    exit(0);
                    break;

                default:
                    break;
            }
        }
    }
}