#pragma once

#include <SDL_rect.h>

#include <cstddef>
#include <memory>

#include "board.hpp"
#include "colors.hpp"
#include "game_context.hpp"
#include "video_context.hpp"

namespace snek::draw {
    void draw_cell(const VideoContext &video_context,
                   const std::shared_ptr<Board> &board,
                   std::size_t cell_y_pos,
                   std::size_t cell_x_pos);
    void draw_board(const VideoContext &video_context,
                    const std::shared_ptr<Board> &board);
    void set_draw_color(SDL_Renderer *renderer, colors::Color color);

    void draw_win_screen(const VideoContext &video_context,
                         const std::shared_ptr<GameContext> &game_context);

    void draw_lose_screen(const VideoContext &video_context,
                          const std::shared_ptr<GameContext> &game_context);

    // accepts an error_message string_view that will be split by \n and
    // displayed as a popup screen
    void draw_error_screen(std::string_view error_message,
                           const VideoContext &video_context,
                           const std::shared_ptr<GameContext> &game_context);

}