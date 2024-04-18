#pragma once

#include "SDL2/SDL.h"
#include "defs.hpp"
#include <vector>

namespace snek {
    class Board {
    private:
        void _draw_cell(snek::App *app, std::size_t y, std::size_t x);
    public:
        void draw_board(snek::App *app);
    };
}