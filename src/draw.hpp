#pragma once

#include "app.hpp"
#include "board.hpp"
#include "cell.hpp"

namespace snek {
    namespace draw {
        void draw_cell(snek::App *app, const snek::Board board,
                       std::size_t y, std::size_t x);
        void draw_board(snek::App *app, const snek::Board board);
    }
}