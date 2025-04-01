#include "app.hpp"

#include <chrono>
#include <cmath>
#include <iostream>

#include "SDL_ttf.h"
#include "colors.hpp"

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

namespace snek {
#ifdef SNEK_ALGORITHM
    App::App()
        : m_board(BOARD_HEIGHT, BOARD_WIDTH),
          m_solver(std::unique_ptr<Board>(&m_board),
                   Dimensions{.height = BOARD_HEIGHT, .width = BOARD_WIDTH}) {
        SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
        SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
        SDL_EventState(SDL_KEYUP, SDL_DISABLE);
#ifdef __EMSCRIPTEN__
        SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#document");
#endif
#else
    App::App() : m_board(game_config::BOARD_HEIGHT, game_config::BOARD_WIDTH) {
#endif
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
                                        game_config::INITIAL_SCREEN_WIDTH,
                                        game_config::INITIAL_SCREEN_HEIGHT,
                                        window_flags);

        if (this->window == nullptr) {
            std::cout << "Failed to open " << game_config::INITIAL_SCREEN_WIDTH
                      << " x " << game_config::INITIAL_SCREEN_HEIGHT
                      << " window: " << SDL_GetError() << "\n";
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

    App::~App() {
        TTF_Quit();
        SDL_Quit();
    }

    void App::run() {
#ifdef __EMSCRIPTEN__
        static App* app_instance = this;

        static auto main_loop_callback = []() {
            if (app_instance->m_cur_state == GameState::Quitting) {
                emscripten_cancel_main_loop();
                return;
            }
            app_instance->m_game_tick();
        };
        emscripten_set_main_loop(main_loop_callback, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
        while (this->m_cur_state != GameState::Quitting) {
            this->m_game_tick();
        }
#endif
    }

    void App::m_game_tick() {
        this->m_prepare_scene();

        std::pair<int, int> raw_direction;
        this->m_do_input(raw_direction);

        if (!m_is_buffering &&
            (raw_direction.first != 0 || raw_direction.second != 0)) {
            this->m_board.set_direction(raw_direction);
            m_is_buffering = true;
        }

        switch (this->m_cur_state) {
            case GameState::Quitting:
                break;
            case GameState::SnakeDead:
                draw::draw_board(this, this->m_board);
                draw::draw_game_over_screen(this);
                this->m_present_scene();
                break;
            case GameState::Running:
                static std::chrono::steady_clock::time_point start =
                    std::chrono::steady_clock::now();

                if ((std::chrono::steady_clock::now() - start) >=
                    std::chrono::microseconds(game_config::MOVE_SPEED)) {
                    start = std::chrono::steady_clock::now();
#ifdef SNEK_ALGORITHM

                    auto next_move = m_solver.get_next_move();
                    this->m_board.set_direction(next_move);
                    m_board.move_snake();
#else
                    switch (this->m_board.move_snake()) {
                        case MoveResultSnakeStatus::Alive:
                            break;
                        case MoveResultSnakeStatus::Dead:
                            this->m_cur_state = GameState::SnakeDead;
                            break;
                    }
#endif
                }
                this->m_is_buffering = false;

                draw::draw_board(this, this->m_board);
                this->m_present_scene();
                break;
        }
        SDL_Delay(5);
    }

    void App::m_prepare_scene() const {
        draw::set_draw_color(this->renderer, colors::BLACK);
        SDL_RenderClear(this->renderer);
    }

    void App::m_present_scene() const {
        SDL_RenderPresent(this->renderer);
    }

    void App::m_do_input(std::pair<int, int>& direction) {
        SDL_Event event;

        static bool press_active = false;
        static int press_start_x = 0;
        static int press_start_y = 0;
        static constexpr int MIN_SWIPE_DISTANCE = 30;

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    this->m_cur_state = GameState::Quitting;
                    break;

                case SDL_KEYDOWN: {
                    if (this->m_cur_state == GameState::SnakeDead &&
                        event.key.keysym.sym == SDLK_r) {
                        this->m_board = Board(game_config::BOARD_HEIGHT,
                                              game_config::BOARD_WIDTH);
                        this->m_cur_state = GameState::Running;
                        return;
                    }
                    if (event.key.keysym.sym == SDLK_UP) {
                        direction = std::pair<int, int>(-1, 0);
                    } else if (event.key.keysym.sym == SDLK_DOWN) {
                        direction = std::pair<int, int>(1, 0);
                    } else if (event.key.keysym.sym == SDLK_LEFT) {
                        direction = std::pair<int, int>(0, -1);
                    } else if (event.key.keysym.sym == SDLK_RIGHT) {
                        direction = std::pair<int, int>(0, 1);
                    }
                    break;
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
                    press_start_x =
                        (int)(event.tfinger.x *
                              (float)SDL_GetWindowSurface(this->window)->w);
                    press_start_y =
                        (int)(event.tfinger.y *
                              (float)SDL_GetWindowSurface(this->window)->h);
                    break;
                }

                case SDL_MOUSEBUTTONUP:
                case SDL_FINGERUP: {
                    if (press_active || press_active && event.button.button ==
                                                            SDL_BUTTON_LEFT) {
                        int press_end_x = 0;
                        int press_end_y = 0;
                        if (press_active &&
                            event.button.button == SDL_BUTTON_LEFT) {
                            press_end_x = event.button.x;
                            press_end_y = event.button.y;
                        } else {
                            press_end_x = (int)event.tfinger.x *
                                          SDL_GetWindowSurface(this->window)->w;
                            press_end_y = (int)event.tfinger.y *
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
                        } else if (std::abs(delta_y) > MIN_SWIPE_DISTANCE) {
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
}