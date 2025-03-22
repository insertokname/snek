#pragma once

#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <vector>

#include "SDL.h"
#include "cell.hpp"
#include "config.hpp"

namespace snek {
    enum class SnakeStatus { Dead, Alive };

    class Board {
    private:
        std::size_t _height, _width;
        std::deque<std::pair<std::size_t, std::size_t>> _snake;
        std::vector<std::vector<snek::Cell>> _mat;
        void _spawn_food();

    public:
        Board(std::size_t height, std::size_t width);

        SnakeStatus move_snake(std::pair<int, int> direction);

        const std::size_t height() const;
        const std::size_t width() const;
        const std::vector<std::vector<snek::Cell>>& mat() const;
    };
}