#include "game_loop.hpp"

#include <SDL_events.h>
#include <SDL_timer.h>

#include <chrono>

#include "board.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "game_context.hpp"
#include "solver.hpp"

namespace snek {
    void GameLoop::game_tick() {
        this->m_prepare_scene();

        std::pair<int, int> raw_direction;
        this->do_input(raw_direction);

        if (!m_is_buffering &&
            (raw_direction.first != 0 || raw_direction.second != 0)) {
            this->m_board->set_direction(raw_direction);
            m_is_buffering = true;
        }

        switch (this->m_game_context->get_cur_game_state()) {
            case GameState::Quit:
                break;
            case GameState::Error:
                draw::draw_board(this->m_video_context, this->m_board);
                draw::draw_error_screen(
                    this->m_game_context->get_error_message(),
                    this->m_video_context,
                    this->m_game_context);
                this->m_present_scene();
                break;
            case GameState::Lost:
                draw::draw_board(this->m_video_context, this->m_board);
                draw::draw_lose_screen(this->m_video_context,
                                       this->m_game_context);
                this->m_present_scene();
                break;
            case GameState::Won:
                draw::draw_board(this->m_video_context, this->m_board);
                draw::draw_win_screen(this->m_video_context,
                                      this->m_game_context);
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
                    if (next_move) {
                        this->m_board->set_direction(next_move.value());
                        m_board->move_snake(this->m_game_context);
                    } else {
                        break;
                    }
#else
                    this->m_board->move_snake(this->m_game_context);
#endif
                }
                this->m_is_buffering = false;

                draw::draw_board(this->m_video_context, this->m_board);
                this->m_present_scene();
                break;
        }
        SDL_Delay(5);
    }

    void GameLoop::m_prepare_scene() const {
        draw::set_draw_color(this->m_video_context.renderer, colors::BLACK);
        SDL_RenderClear(this->m_video_context.renderer);
    }

    void GameLoop::m_present_scene() const {
        SDL_RenderPresent(this->m_video_context.renderer);
    }

    void GameLoop::do_input(std::pair<int, int>& direction) {
        SDL_Event event;

        static bool press_active = false;
        static int press_start_x = 0;
        static int press_start_y = 0;
        static constexpr int MIN_SWIPE_DISTANCE = 30;

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    this->m_game_context->set_cur_game_state(GameState::Quit);
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
                    press_start_x = static_cast<int>(
                        event.tfinger.x * (float)SDL_GetWindowSurface(
                                              this->m_video_context.window)
                                              ->w);
                    press_start_y = static_cast<int>(
                        event.tfinger.y * (float)SDL_GetWindowSurface(
                                              this->m_video_context.window)
                                              ->h);
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
                            press_end_x = static_cast<int>(event.tfinger.x) *
                                          SDL_GetWindowSurface(
                                              this->m_video_context.window)
                                              ->w;
                            press_end_y = static_cast<int>(event.tfinger.y) *
                                          SDL_GetWindowSurface(
                                              this->m_video_context.window)
                                              ->h;
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