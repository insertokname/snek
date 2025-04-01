#pragma once

#include <SDL_rect.h>

#include <cstddef>

#include "app.hpp"
#include "board.hpp"
#include "colors.hpp"

namespace snek::draw {
    void draw_cell(snek::App *app,
                   const snek::Board &board,
                   std::size_t y,
                   std::size_t x);
    void draw_board(snek::App *app, const snek::Board &board);
    void set_draw_color(SDL_Renderer *renderer, snek::colors::Color color);
    void draw_game_over_screen(snek::App *app);
}