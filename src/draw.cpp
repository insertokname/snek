#include "draw.hpp"

#include <SDL_pixels.h>
#include <SDL_surface.h>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string_view>

#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "board.hpp"
#include "cell.hpp"
#include "colors.hpp"
#include "config.hpp"
#include "game_context.hpp"
#include "rect_tools.hpp"
#include "text.hpp"
#include "video_context.hpp"

namespace snek::draw {
    void set_draw_color(SDL_Renderer *renderer, colors::Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    void draw_cell(const VideoContext &video_context,
                   const std::shared_ptr<Board> &board,
                   std::size_t cell_y_pos,
                   std::size_t cell_x_pos) {
        // TODO: move this setup out of the draw_cell function to improve
        // performance
        SDL_Rect screen_rect;
        rect_tools::get_screen_rect(video_context.window, screen_rect);

        SDL_Rect out_game_board_rect;
        rect_tools::apply_rect_style(out_game_board_rect,
                                     screen_rect,
                                     rect_tools::SymmetrictPaddingRectStyle(
                                         {.x_padding = game_config::PADDING,
                                          .y_padding = game_config::PADDING}));

        const std::size_t cell_width =
            out_game_board_rect.w / board->get_size().width;
        const std::size_t cell_height =
            out_game_board_rect.h / board->get_size().height;
        const std::size_t cell_size = std::min(cell_height, cell_width);

        out_game_board_rect.w =
            static_cast<int>(cell_size * board->get_size().width);
        out_game_board_rect.h =
            static_cast<int>(cell_size * board->get_size().height);

        rect_tools::apply_rect_style(
            out_game_board_rect,
            screen_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::BOARD_XY_POS,
                .relative_y_pos = gui_config::BOARD_XY_POS,
            }));

        const int screen_x_pos =
            out_game_board_rect.x + static_cast<int>(cell_size * cell_x_pos);
        const int screen_y_pos =
            out_game_board_rect.y + static_cast<int>(cell_size * cell_y_pos);

        SDL_Rect cell_rect;
        cell_rect.x = screen_x_pos;
        cell_rect.y = screen_y_pos;
        cell_rect.w = (int)cell_size;
        cell_rect.h = (int)cell_size;

        switch (board->mat()[cell_y_pos][cell_x_pos]) {
            case Cell::Empty:
                break;

            case Cell::Tail:
                set_draw_color(video_context.renderer, colors::TAIL_GREEN);
                SDL_RenderFillRect(video_context.renderer, &cell_rect);
                break;

            case Cell::Body:
                set_draw_color(video_context.renderer, colors::BODY_GREEN);
                SDL_RenderFillRect(video_context.renderer, &cell_rect);
                break;

            case Cell::Head:
                set_draw_color(video_context.renderer, colors::HEAD_GREEN);
                SDL_RenderFillRect(video_context.renderer, &cell_rect);
                break;

            case Cell::Food:
                set_draw_color(video_context.renderer, colors::FOOD_RED);
                SDL_RenderFillRect(video_context.renderer, &cell_rect);
                break;

            default:
                set_draw_color(video_context.renderer, colors::BORDER_WHITE);
                SDL_RenderFillRect(video_context.renderer, &cell_rect);
                break;
        }
        set_draw_color(video_context.renderer, colors::BORDER_WHITE);
        SDL_RenderDrawRect(video_context.renderer, &cell_rect);
    }

    void draw_board(const VideoContext &video_context,
                    const std::shared_ptr<Board> &board) {
        for (std::size_t i = 0; i < board->get_size().height; i++) {
            for (std::size_t j = 0; j < board->get_size().width; j++) {
                draw_cell(video_context, board, i, j);
            }
        }
    }

    void draw_game_over_screen(
        const VideoContext &video_context,
        const std::shared_ptr<GameContext> &game_context) {
        SDL_Rect screen_rect;
        rect_tools::get_screen_rect(video_context.window, screen_rect);

        SDL_Rect game_over_background_rect;
        rect_tools::apply_rect_style(
            game_over_background_rect,
            screen_rect,
            rect_tools::SymmetricRelativeRectStyle({
                .pos_percent = gui_config::POPUP_SCREEN_XY_POS,
                .size_percent = gui_config::POPUP_SCREEN_XY_POS,
            }));

        SDL_Rect game_over_border_background_rect;
        rect_tools::apply_rect_style(game_over_border_background_rect,
                                     game_over_background_rect,
                                     rect_tools::BorderRectStyle({
                                         .border_thickness = 4,
                                     }));

        set_draw_color(video_context.renderer, colors::BORDER_WHITE);
        SDL_RenderFillRect(video_context.renderer,
                           &game_over_border_background_rect);

        set_draw_color(video_context.renderer, colors::BLACK);
        SDL_RenderFillRect(video_context.renderer, &game_over_background_rect);

        TTF_Font *upheavtt =
            TTF_OpenFont((game_context->get_exe_path().parent_path() /
                          "assets" / "upheavtt.ttf")
                             .string()
                             .c_str(),
                         gui_config::POPUP_SCREEN_FONT_SIZE);

        text::TextStyleParams style = {
            .color = colors::BORDER_WHITE,
            .font = upheavtt,
        };

        text::draw_text_in_rect(
            text::Text("game over!", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                .relative_y_pos = gui_config::POPUP_SCREEN_TITLE_Y_POS,
            }),
            video_context.renderer);

        text::draw_text_in_rect(
            text::Text("press 'r'", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                .relative_y_pos = gui_config::POPUP_SCREEN_SUBHEADING1_Y_POS,
            }),
            video_context.renderer);

        text::draw_text_in_rect(
            text::Text("to try again!", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                .relative_y_pos = gui_config::POPUP_SCREEN_SUBHEADING2_Y_POS,
            }),
            video_context.renderer);

        TTF_CloseFont(upheavtt);
    }

    void draw_win_screen(const VideoContext &video_context,
                         const std::shared_ptr<GameContext> &game_context) {
        SDL_Rect screen_rect;
        rect_tools::get_screen_rect(video_context.window, screen_rect);

        SDL_Rect game_over_background_rect;
        rect_tools::apply_rect_style(
            game_over_background_rect,
            screen_rect,
            rect_tools::SymmetricRelativeRectStyle({
                .pos_percent = gui_config::POPUP_SCREEN_XY_POS,
                .size_percent = gui_config::POPUP_SCREEN_XY_POS,
            }));

        SDL_Rect game_over_border_background_rect;
        rect_tools::apply_rect_style(game_over_border_background_rect,
                                     game_over_background_rect,
                                     rect_tools::BorderRectStyle({
                                         .border_thickness = 4,
                                     }));

        set_draw_color(video_context.renderer, colors::BORDER_WHITE);
        SDL_RenderFillRect(video_context.renderer,
                           &game_over_border_background_rect);

        set_draw_color(video_context.renderer, colors::BLACK);
        SDL_RenderFillRect(video_context.renderer, &game_over_background_rect);

        TTF_Font *upheavtt =
            TTF_OpenFont((game_context->get_exe_path().parent_path() /
                          "assets" / "upheavtt.ttf")
                             .string()
                             .c_str(),
                         gui_config::POPUP_SCREEN_FONT_SIZE);

        text::TextStyleParams style = {
            .color = colors::BORDER_WHITE,
            .font = upheavtt,
        };

        text::draw_text_in_rect(
            text::Text("You won!", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                .relative_y_pos = gui_config::POPUP_SCREEN_TITLE_Y_POS,
            }),
            video_context.renderer);

        text::draw_text_in_rect(
            text::Text("press 'r'", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                .relative_y_pos = gui_config::POPUP_SCREEN_SUBHEADING1_Y_POS,
            }),
            video_context.renderer);

        text::draw_text_in_rect(
            text::Text("to restart game!", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                .relative_y_pos = gui_config::POPUP_SCREEN_SUBHEADING2_Y_POS,
            }),
            video_context.renderer);

        TTF_CloseFont(upheavtt);
    }
}