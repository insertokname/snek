#pragma once

#include "SDL2/SDL.h"
#include "defs.hpp"
#include "path.hpp"
#include <vector>
#include <deque>
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace snek {
    class Board {
    private:
        std::vector<std::vector<int>> _path;
        std::size_t _height, _width;
        std::deque <std::pair<std::size_t, std::size_t>> _snake;
        enum Cell { empty, tail, body, head, food };
        std::vector <std::vector<Board::Cell>> _mat;
        void _draw_cell(snek::App *app, std::size_t y, std::size_t x);
        void _spawn_food();
    public:
        Board(std::size_t height, std::size_t width);
        void draw_board(snek::App *app);

        //returns 1 if the snake is dead
        //return 2 if the snake should keep course
        int move_snake(std::pair<int, int> direction);
        void auto_move_snake();
    };
}