#include "app.hpp"

#include "draw.hpp"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#ifdef SNEK_ALGORITHM
snek::App::App()
    : _board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH),
      _solver(_board, snek::BOARD_HEIGHT, snek::BOARD_WIDTH) {
#else
snek::App::App() : _board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH) {
#endif
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

    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
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
    static snek::App* app_instance = this;

    static auto main_loop_callback = []() {
        app_instance->game_tick();
    };
    emscripten_set_main_loop(main_loop_callback, 0, 1);
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
#ifdef SNEK_ALGORITHM

        auto next_move = _solver.get_next_move();
        _board.move_snake(next_move);
#else
        buffering = 0;
        switch (this->_board.move_snake(direction)) {
            case snek::SnakeStatus::Alive:
                break;
            case snek::SnakeStatus::Dead:
                std::cout << "you died!";
                exit(0);
                break;
        }
#endif
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

    static bool pressActive = false;
    static int pressStartX = 0;
    static int pressStartY = 0;
    static const int MIN_SWIPE_DISTANCE = 30;

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

            case SDL_MOUSEBUTTONDOWN: {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    pressActive = true;
                    pressStartX = event.button.x;
                    pressStartY = event.button.y;
                }
                break;
            }

            case SDL_FINGERDOWN: {
                pressActive = true;
                pressStartX = event.tfinger.x *
                              SDL_GetWindowSurface(this->window)->w;
                pressStartY = event.tfinger.y *
                              SDL_GetWindowSurface(this->window)->h;
                break;
            }

            case SDL_MOUSEBUTTONUP:
            case SDL_FINGERUP: {
                if (pressActive ||
                    pressActive &&
                        event.button.button == SDL_BUTTON_LEFT) {
                    int pressEndX;
                    int pressEndY;
                    if (pressActive &&
                        event.button.button == SDL_BUTTON_LEFT) {
                        pressEndX = event.button.x;
                        pressEndY = event.button.y;
                    } else {
                        pressEndX =
                            event.tfinger.x *
                            SDL_GetWindowSurface(this->window)->w;
                        pressEndY =
                            event.tfinger.y *
                            SDL_GetWindowSurface(this->window)->h;
                    }
                    int deltaX = pressEndX - pressStartX;
                    int deltaY = pressEndY - pressStartY;

                    if (abs(deltaX) > abs(deltaY) &&
                        abs(deltaX) > MIN_SWIPE_DISTANCE) {
                        if (deltaX > 0) {
                            direction = std::pair<int, int>(0, 1);
                        } else {
                            direction = std::pair<int, int>(0, -1);
                        }
                    } else if (abs(deltaY) > MIN_SWIPE_DISTANCE) {
                        if (deltaY > 0) {
                            direction = std::pair<int, int>(1, 0);
                        } else {
                            direction = std::pair<int, int>(-1, 0);
                        }
                    }

                    pressActive = false;
                }
                break;
            }

            default:
                break;
        }
    }
}