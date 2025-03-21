#pragma once

#include <iostream>

#include "SDL2/SDL.h"
#include "board.hpp"
#include "cell.hpp"
#include "config.hpp"

namespace snek {
    class App {
    public:
        SDL_Renderer *renderer;
        SDL_Window *window;
        App();
        void run();

    private:
        Board _board;
        void prepareScene();
        void presentScene();
        void doInput(std::pair<int, int> &direction);
    };
}