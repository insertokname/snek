#pragma once

#include <cstdint>

#include "SDL_render.h"
#include "board.hpp"

#ifdef SNEK_ALGORITHM
#include "solver.hpp"
#endif

namespace snek {
    enum class GameState : std::uint8_t {
        Running,
        SnakeDead,
        Quitting,
    };

    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
        App();
        ~App();
        void run();

    private:
        GameState m_cur_state = GameState::Running;
        Board m_board;
#ifdef SNEK_ALGORITHM
        Solver m_solver;
#endif
        bool m_is_buffering = false;
        void m_game_tick();
        void m_prepare_scene() const;
        void m_present_scene() const;
        void m_do_input(std::pair<int, int> &direction);
    };
}