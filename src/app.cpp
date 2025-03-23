#include "app.hpp"

#include "draw.hpp"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

snek::App::App() : _board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH) {
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = SDL_WINDOW_RESIZABLE;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't initialize SDL: " << SDL_GetError()
                  << "\n";
        exit(1);
    }

    this->window = SDL_CreateWindow(
        "snek", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        snek::INITIAL_SCREEN_WIDTH, snek::INITIAL_SCREEN_HEIGHT,
        windowFlags);

    if (!this->window) {
        std::cout << "Failed to open " << snek::INITIAL_SCREEN_WIDTH
                  << " x " << snek::INITIAL_SCREEN_HEIGHT
                  << " window: " << SDL_GetError() << "\n";
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    this->renderer =
        SDL_CreateRenderer(this->window, -1, rendererFlags);

    if (!this->renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError()
                  << "\n";
        exit(1);
    }
}

void snek::App::run() {
#ifdef __EMSCRIPTEN__
    // Create a static instance pointer that can be accessed from the
    // callback
    static snek::App* app_instance = this;

    // Create a static function that emscripten can call
    static auto main_loop_callback = []() {
        app_instance->game_tick();
    };
    emscripten_set_main_loop(main_loop_callback, 0, 1);

    // emscripten_set_main_loop(this->game_tick(), 0, 1);
#endif

#ifndef __EMSCRIPTEN__
    while (true) this->game_tick();
#endif
}

void snek::App::game_tick() {
    this->prepareScene();

    std::pair<int, int> raw_input;
    this->doInput(raw_input);

    if (!buffering && (raw_input.first || raw_input.second)) {
        direction = raw_input;
        buffering = 1;
    }

    if ((std::chrono::steady_clock::now() - start) >=
        std::chrono::microseconds(snek::MOVE_SPEED)) {
        start = std::chrono::steady_clock::now();
        buffering = 0;
        switch (this->_board.move_snake(direction)) {
            case snek::SnakeStatus::Alive:
                break;
            case snek::SnakeStatus::Dead:
                std::cout << "you died!";
                exit(0);
                break;
        }
    }

    snek::draw::draw_board(this, this->_board);
    this->presentScene();
    SDL_Delay(5);
}

void snek::App::prepareScene() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
}

void snek::App::presentScene() {
    SDL_RenderPresent(this->renderer);
}

void snek::App::doInput(std::pair<int, int>& direction) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_UP) {
                    direction = std::pair<int, int>(-1, 0);
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    direction = std::pair<int, int>(1, 0);
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    direction = std::pair<int, int>(0, -1);
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    direction = std::pair<int, int>(0, 1);
                }
            }
            default:
                break;
        }
    }
}