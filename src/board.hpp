#pragma once

#include "SDL2/SDL.h"
#include "defs.hpp"
#include "config.hpp"
#include <vector>
#include <deque>
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace snek {
    class Board {
    private:
        std::size_t _height, _width;
        std::deque <std::pair<std::size_t, std::size_t>> _snake;
        std::vector <std::vector<snek::Cell>> _mat;
        void _spawn_food();
    public:
        Board(std::size_t height, std::size_t width);

        //returns 1 if the snake is dead
        //return 2 if the snake should keep course
        int move_snake(std::pair<int, int> direction);

        const std::size_t height() const;
        const std::size_t width() const;
        const std::vector <std::vector<snek::Cell>>& mat()const;
    };
}