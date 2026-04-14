#include "game_loop.hpp"

#include <SDL_events.h>
#include <SDL_timer.h>

#include <chrono>
#include <cmath>

#include "board.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "game_context.hpp"
#include "solver.hpp"

namespace snek {
    void GameLoop::game_tick() {
        const auto now = std::chrono::steady_clock::now();

        std::pair<int, int> raw_direction = {0, 0};
        static auto last_input_poll =
            now - std::chrono::microseconds(game_config::INPUT_POLL_INTERVAL_US);

        if ((now - last_input_poll) >=
            std::chrono::microseconds(game_config::INPUT_POLL_INTERVAL_US)) {
            this->do_input(raw_direction);
            last_input_poll = now;
        }

        if (!m_is_buffering &&
            (raw_direction.first != 0 || raw_direction.second != 0)) {
            this->m_board->set_direction(raw_direction);
            m_is_buffering = true;
        }

        static auto move_start = now;
        static auto last_render = now;
        static GameState previous_state =
            this->m_game_context->get_cur_game_state();

        const GameState current_state =
            this->m_game_context->get_cur_game_state();

        if (current_state != previous_state) {
            m_force_render = true;
            previous_state = current_state;
        }

        const bool render_due =
            (now - last_render) >=
            std::chrono::microseconds(game_config::RENDER_INTERVAL_US);

        switch (current_state) {
            case GameState::Quit:
                break;
            case GameState::Error:
                if (m_force_render) {
                    this->m_prepare_scene();
                    draw::draw_board(this->m_video_context, this->m_board);
                    draw::draw_error_screen(
                        this->m_game_context->get_error_message(),
                        this->m_video_context,
                        this->m_game_context);
                    this->m_present_scene();
                    last_render = now;
                    m_force_render = false;
                }
                break;
            case GameState::Lost:
                if (m_force_render) {
                    this->m_prepare_scene();
                    draw::draw_board(this->m_video_context, this->m_board);
                    draw::draw_lose_screen(this->m_video_context,
                                           this->m_game_context);
                    this->m_present_scene();
                    last_render = now;
                    m_force_render = false;
                }
                break;
            case GameState::Won:
                this->m_prepare_scene();
                draw::draw_board(this->m_video_context, this->m_board);
                draw::draw_win_screen(this->m_video_context,
                                      this->m_game_context);
                this->m_present_scene();
#ifdef SNEK_PERF
                this->m_game_context->set_cur_game_state(GameState::Quit);
#endif
                break;
            case GameState::Running: {
                bool moved = false;

                if ((now - move_start) >=
                    std::chrono::microseconds(game_config::MOVE_SPEED)) {
                    move_start = now;
#ifdef SNEK_ALGORITHM
                    auto next_move = m_solver.get_next_move();
                    if (next_move) {
                        this->m_board->set_direction(next_move.value());
                        m_board->move_snake(this->m_game_context);
                        moved = true;
                    } else {
                        break;
                    }
#else
                    this->m_board->move_snake(this->m_game_context);
                    moved = true;
#endif
                }
                this->m_is_buffering = false;

                if ((moved || m_force_render) && render_due) {
                    this->m_prepare_scene();
                    draw::draw_board(this->m_video_context, this->m_board);
                    this->m_present_scene();
                    last_render = now;
                    m_force_render = false;
                }
                break;
            }
        }

#ifndef SNEK_ALGORITHM
        SDL_Delay(1);
#endif
    }

    void GameLoop::m_prepare_scene() const {
        draw::set_draw_color(this->m_video_context.renderer, colors::BLACK);
        SDL_RenderClear(this->m_video_context.renderer);
    }

    void GameLoop::m_present_scene() const {
        SDL_RenderPresent(this->m_video_context.renderer);
    }

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    void GameLoop::do_input(std::pair<int, int>& direction) {
        SDL_Event event;

        static bool press_active = false;
        static int press_start_x = 0;
        static int press_start_y = 0;

        int window_width = 0;
        int window_height = 0;
        SDL_GetWindowSize(
            this->m_video_context.window, &window_width, &window_height);

        auto touch_to_screen = [&](float x, float y) {
            return std::pair<int, int>(
                static_cast<int>(x * static_cast<float>(window_width)),
                static_cast<int>(y * static_cast<float>(window_height)));
        };

        auto direction_from_swipe = [&](int press_end_x, int press_end_y) {
            const int delta_x = press_end_x - press_start_x;
            const int delta_y = press_end_y - press_start_y;

            if (std::abs(delta_x) > std::abs(delta_y) &&
                std::abs(delta_x) > game_config::MIN_SWIPE_DISTANCE) {
                if (delta_x > 0) {
                    direction = std::pair<int, int>(0, 1);
                } else {
                    direction = std::pair<int, int>(0, -1);
                }
                m_force_render = true;
            } else if (std::abs(delta_y) > game_config::MIN_SWIPE_DISTANCE) {
                if (delta_y > 0) {
                    direction = std::pair<int, int>(1, 0);
                } else {
                    direction = std::pair<int, int>(-1, 0);
                }
                m_force_render = true;
            }
        };

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    this->m_game_context->set_cur_game_state(GameState::Quit);
                    break;

                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                        event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
                        event.window.event == SDL_WINDOWEVENT_EXPOSED) {
                        m_force_render = true;
                    }
                    break;

                case SDL_KEYDOWN: {
                    if ((this->m_game_context->get_cur_game_state() ==
                             GameState::Lost ||
                         this->m_game_context->get_cur_game_state() ==
                             GameState::Won ||
                         this->m_game_context->get_cur_game_state() ==
                             GameState::Error) &&
                        event.key.keysym.sym == SDLK_r) {
                        this->m_game_context->set_cur_game_state(
                            GameState::Running);
                        *this->m_board = Board(this->m_dimensions);
#ifdef SNEK_ALGORITHM
                        this->m_solver = Solver(this->m_board, m_game_context);
#endif
                        m_force_render = true;
                        return;
                    }

                    if (event.key.keysym.sym == SDLK_UP) {
                        direction = std::pair<int, int>(-1, 0);
                        m_force_render = true;
                    } else if (event.key.keysym.sym == SDLK_DOWN) {
                        direction = std::pair<int, int>(1, 0);
                        m_force_render = true;
                    } else if (event.key.keysym.sym == SDLK_LEFT) {
                        direction = std::pair<int, int>(0, -1);
                        m_force_render = true;
                    } else if (event.key.keysym.sym == SDLK_RIGHT) {
                        direction = std::pair<int, int>(0, 1);
                        m_force_render = true;
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
                    const auto [x, y] =
                        touch_to_screen(event.tfinger.x, event.tfinger.y);
                    press_start_x = x;
                    press_start_y = y;
                    break;
                }

                case SDL_MOUSEBUTTONUP: {
                    if (press_active &&
                        event.button.button == SDL_BUTTON_LEFT) {
                        direction_from_swipe(event.button.x, event.button.y);
                        press_active = false;
                    }
                    break;
                }

                case SDL_FINGERUP: {
                    if (press_active) {
                        const auto [x, y] =
                            touch_to_screen(event.tfinger.x, event.tfinger.y);
                        direction_from_swipe(x, y);
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