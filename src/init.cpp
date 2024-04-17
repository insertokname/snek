#include "init.hpp"

void initSDL(snek::App *app) {
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't initialize SDL: " << SDL_GetError() << "\n";
        exit(1);
    }

    app->window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, snek::SCREEN_WIDTH, snek::SCREEN_HEIGHT, windowFlags);

    if (!app->window) {
        std::cout << "Failed to open " << snek::SCREEN_WIDTH << " x " << snek::SCREEN_HEIGHT << " window: " << SDL_GetError() << "\n";
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);

    if (!app->renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << "\n";
        exit(1);
    }
}