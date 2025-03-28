#include "app.hpp"

#include <chrono>
#include <cmath>
#include <iostream>

#include "colors.hpp"
#include "dimensions.hpp"

#ifdef SNEK_ALGORITHM
#include <memory>
#endif
#include <utility>

#include "SDL.h"
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_hints.h"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "board.hpp"
#include "config.hpp"
#include "draw.hpp"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#ifdef SNEK_ALGORITHM
snek::App::App()
    : m_board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH),
      m_solver(std::unique_ptr<Board>(&m_board),
               snek::Dimensions{.height = snek::BOARD_HEIGHT,
                                .width = snek::BOARD_WIDTH}) {
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
#ifdef __EMSCRIPTEN__
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#document");
#endif
#else
snek::App::App() : m_board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH) {
#endif
    int renderer_flags = SDL_RENDERER_ACCELERATED;
    int window_flags = SDL_WINDOW_RESIZABLE;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't initialize SDL: " << SDL_GetError()
                  << "\n";
        exit(1);
    }

    this->window = SDL_CreateWindow(
        "snek", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        snek::INITIAL_SCREEN_WIDTH, snek::INITIAL_SCREEN_HEIGHT,
        window_flags);

    if (this->window == nullptr) {
        std::cout << "Failed to open " << snek::INITIAL_SCREEN_WIDTH
                  << " x " << snek::INITIAL_SCREEN_HEIGHT
                  << " window: " << SDL_GetError() << "\n";
        exit(1);
    }

    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    this->renderer =
        SDL_CreateRenderer(this->window, -1, renderer_flags);

    if (this->renderer == nullptr) {
        std::cout << "Failed to create renderer: " << SDL_GetError()
                  << "\n";
        exit(1);
    }
}

void snek::App::run() {
#ifdef __EMSCRIPTEN__
    static snek::App* app_instance = this;

    static auto main_loop_callback = []() {
        app_instance->m_game_tick();
    };
    emscripten_set_main_loop(main_loop_callback, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
    while (true) {
        this->m_game_tick();
    }
#endif
}

void snek::App::m_game_tick() {
    this->m_prepare_scene();

    std::pair<int, int> raw_input;
    this->m_do_input(raw_input);

    static std::chrono::steady_clock::time_point start =
        std::chrono::steady_clock::now();
    static std::pair<int, int> direction;
    static bool buffering = false;

    if (!buffering &&
        (raw_input.first != 0 || raw_input.second != 0)) {
        direction = raw_input;
        buffering = true;
    }

    if ((std::chrono::steady_clock::now() - start) >=
        std::chrono::microseconds(snek::MOVE_SPEED)) {
        start = std::chrono::steady_clock::now();
#ifdef SNEK_ALGORITHM

        auto next_move = m_solver.get_next_move();
        m_board.move_snake(next_move);
#else
        buffering = false;
        switch (this->m_board.move_snake(direction)) {
            case snek::SnakeStatus::Alive:
                break;
            case snek::SnakeStatus::Dead:
                std::cout << "you died!";
                exit(0);
                break;
        }
#endif
    }

    snek::draw::draw_board(this, this->m_board);
    this->m_present_scene();
    SDL_Delay(5);
}

void snek::App::m_prepare_scene() const {
    snek::draw::set_draw_color(this->renderer, snek::colors::BLACK);
    SDL_RenderClear(this->renderer);
}

void snek::App::m_present_scene() const {
    SDL_RenderPresent(this->renderer);
}

void snek::App::m_do_input(std::pair<int, int>& direction) const {
    SDL_Event event;

    static bool press_active = false;
    static int press_start_x = 0;
    static int press_start_y = 0;
    static constexpr int MIN_SWIPE_DISTANCE = 30;

    while (SDL_PollEvent(&event) != 0) {
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
                    press_active = true;
                    press_start_x = event.button.x;
                    press_start_y = event.button.y;
                }
                break;
            }

            case SDL_FINGERDOWN: {
                press_active = true;
                press_start_x = (int)event.tfinger.x *
                                SDL_GetWindowSurface(this->window)->w;
                press_start_y = (int)event.tfinger.y *
                                SDL_GetWindowSurface(this->window)->h;
                break;
            }

            case SDL_MOUSEBUTTONUP:
            case SDL_FINGERUP: {
                if (press_active ||
                    press_active &&
                        event.button.button == SDL_BUTTON_LEFT) {
                    int press_end_x = 0;
                    int press_end_y = 0;
                    if (press_active &&
                        event.button.button == SDL_BUTTON_LEFT) {
                        press_end_x = event.button.x;
                        press_end_y = event.button.y;
                    } else {
                        press_end_x =
                            (int)event.tfinger.x *
                            SDL_GetWindowSurface(this->window)->w;
                        press_end_y =
                            (int)event.tfinger.y *
                            SDL_GetWindowSurface(this->window)->h;
                    }
                    const int delta_x = press_end_x - press_start_x;
                    const int delta_y = press_end_y - press_start_y;

                    if (std::abs(delta_x) > std::abs(delta_y) &&
                        std::abs(delta_x) > MIN_SWIPE_DISTANCE) {
                        if (delta_x > 0) {
                            direction = std::pair<int, int>(0, 1);
                        } else {
                            direction = std::pair<int, int>(0, -1);
                        }
                    } else if (std::abs(delta_y) >
                               MIN_SWIPE_DISTANCE) {
                        if (delta_y > 0) {
                            direction = std::pair<int, int>(1, 0);
                        } else {
                            direction = std::pair<int, int>(-1, 0);
                        }
                    }

                    press_active = false;
                }
                break;
            }

            default:
                break;
        }
    }
}