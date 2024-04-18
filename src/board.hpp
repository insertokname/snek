#pragma once

#include "SDL2/SDL.h"
#include "defs.hpp"
#include <vector>

namespace snek {
    class Board {
    private:
        std::size_t _height, _width;
        enum Cell { empty, tail, body, head, food };
        std::vector <std::vector<Board::Cell>> mat;
        void _draw_cell(snek::App *app, std::size_t y, std::size_t x);
    public:
        Board(std::size_t height, std::size_t width);
        void draw_board(snek::App *app);
    };
}