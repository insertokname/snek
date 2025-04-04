#pragma once

#include <SDL_rect.h>

#include <cstddef>

#include "board.hpp"
#include "colors.hpp"
#include "video_context.hpp"

namespace snek::draw {
    void draw_cell(const VideoContext &video_context,
                   const Board &board,
                   std::size_t cell_y_pos,
                   std::size_t cell_x_pos);
    void draw_board(const snek::VideoContext &video_context,
                    const snek::Board &board);
    void set_draw_color(SDL_Renderer *renderer, snek::colors::Color color);
    void draw_game_over_screen(snek::VideoContext &video_context);
}