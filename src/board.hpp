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
    public:
        Board(std::size_t height, std::size_t width);

        SnakeStatus move_snake(std::pair<int, int> direction);

        const std::size_t height() const;
        const std::size_t width() const;
        const std::pair<std::size_t, std::size_t>& get_food_location()
            const;
        const std::vector<std::vector<snek::Cell>>& mat() const;
        const std::deque<std::pair<std::size_t, std::size_t>>&
        get_snake() const;

    private:
        std::size_t _height, _width;
        std::deque<std::pair<std::size_t, std::size_t>> _snake;
        std::vector<std::vector<snek::Cell>> _mat;
        std::pair<std::size_t, std::size_t> _food_location;
        void _spawn_food();
    };
}