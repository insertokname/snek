#pragma once

#include <chrono>
#include <iostream>

#include "SDL.h"
#include "board.hpp"
#include "cell.hpp"
#include "config.hpp"
#include "solver.hpp"

namespace snek {
    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
        App();
        void run();

    private:
        Board _board;
#ifdef SNEK_ALGORITHM
        Solver _solver;
#endif
        void game_tick();
        void prepareScene();
        void presentScene();
        void doInput(std::pair<int, int> &direction);
        std::chrono::steady_clock::time_point start =
            std::chrono::steady_clock::now();
        std::pair<int, int> direction;
        bool buffering = 0;
    };
}