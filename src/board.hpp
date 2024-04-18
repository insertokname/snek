#pragma once

#include "SDL2/SDL.h"
#include "defs.hpp"
#include <vector>
#include <deque>

namespace snek {
    class Board {
    private:
        std::size_t _height, _width;
        std::deque <std::pair<std::size_t, std::size_t>> _snake;
        enum Cell { empty, tail, body, head, food };
        std::vector <std::vector<Board::Cell>> _mat;
        void _draw_cell(snek::App *app, std::size_t y, std::size_t x);
    public:
        Board(std::size_t height, std::size_t width);
        void draw_board(snek::App *app);

        //returns 1 if the snake is dead
        //dead = overlaping or outside boundry
        bool move_snake(std::pair<int, int> direction);
    };
}