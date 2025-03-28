#pragma once

#include <SDL_render.h>

#include "board.hpp"

#ifdef SNEK_ALGORITHM
#include "solver.hpp"
#endif

namespace snek {
    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
        App();
        void run();

    private:
        Board m_board;
#ifdef SNEK_ALGORITHM
        Solver m_solver;
#endif
        void m_game_tick();
        void m_prepare_scene() const;
        void m_present_scene() const;
        void m_do_input(std::pair<int, int> &direction) const;
    };
}