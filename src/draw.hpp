#pragma once

#include "defs.hpp"
#include "board.hpp"

namespace snek {
    void draw_cell(snek::App *app, const snek::Board board, std::size_t y, std::size_t x);
    void draw_board(snek::App *app, const snek::Board board);
}