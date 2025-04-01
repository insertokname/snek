#include "draw.hpp"

#include <SDL_pixels.h>
#include <SDL_surface.h>

#include <algorithm>
#include <cstddef>
#include <string_view>

#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "app.hpp"
#include "board.hpp"
#include "cell.hpp"
#include "colors.hpp"
#include "config.hpp"
#include "rect_tools.hpp"
#include "text.hpp"

namespace snek::draw {
    void set_draw_color(SDL_Renderer *renderer, colors::Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    void draw_cell(App *app, const Board &board, std::size_t y, std::size_t x) {
        int width = 0, height = 0;
        SDL_GetWindowSize(app->window, &width, &height);
        width -= game_config::PADDING * 2;
        height -= game_config::PADDING * 2;
        const std::size_t cell_width = width / game_config::BOARD_WIDTH;
        const std::size_t cell_height = height / game_config::BOARD_HEIGHT;
        const std::size_t cell_size = std::min(cell_height, cell_width);

        const double width_start_offset =
            ((width - ((double)(cell_size)*game_config::BOARD_WIDTH)) / 2.0) +
            game_config::PADDING;
        const double height_start_offset =
            ((height - ((double)(cell_size)*game_config::BOARD_HEIGHT)) / 2.0) +
            game_config::PADDING;

        SDL_Rect rect;
        rect.x = (int)x * (int)cell_size + (int)width_start_offset;
        rect.y = (int)y * (int)cell_size + (int)height_start_offset;
        rect.w = (int)cell_size;
        rect.h = (int)cell_size;

        switch (board.mat()[y][x]) {
            case Cell::Empty:
                break;

            case Cell::Tail:
                set_draw_color(app->renderer, colors::TAIL_GREEN);
                SDL_RenderFillRect(app->renderer, &rect);
                break;

            case Cell::Body:
                set_draw_color(app->renderer, colors::BODY_GREEN);
                SDL_RenderFillRect(app->renderer, &rect);
                break;

            case Cell::Head:
                set_draw_color(app->renderer, colors::HEAD_GREEN);
                SDL_RenderFillRect(app->renderer, &rect);
                break;

            case Cell::Food:
                set_draw_color(app->renderer, colors::FOOD_RED);
                SDL_RenderFillRect(app->renderer, &rect);
                break;

            default:
                set_draw_color(app->renderer, colors::BORDER_WHITE);
                SDL_RenderFillRect(app->renderer, &rect);
                break;
        }
        set_draw_color(app->renderer, colors::BORDER_WHITE);
        SDL_RenderDrawRect(app->renderer, &rect);
    }

    void draw_board(App *app, const Board &board) {
        for (std::size_t i = 0; i < board.height(); i++) {
            for (std::size_t j = 0; j < board.width(); j++) {
                draw_cell(app, board, i, j);
            }
        }
    }

    void draw_game_over_screen(App *app) {
        SDL_Rect screen_rect{
            .x = 0,
            .y = 0,
        };
        SDL_GetWindowSize(app->window, &screen_rect.w, &screen_rect.h);

        SDL_Rect game_over_background_rect;
        rect_tools::apply_rect_style(
            game_over_background_rect,
            screen_rect,
            rect_tools::SymmetricRelativeRectStyle({
                .pos_percent = gui_config::GAME_OVER_XY_POS,
                .size_percent = gui_config::GAME_OVER_XY_POS,
            }));

        SDL_Rect game_over_border_background_rect;
        rect_tools::apply_rect_style(game_over_border_background_rect,
                                     game_over_background_rect,
                                     rect_tools::BorderRectStyle({
                                         .border_thickness = 4,
                                     }));

        set_draw_color(app->renderer, colors::BORDER_WHITE);
        SDL_RenderFillRect(app->renderer, &game_over_border_background_rect);

        set_draw_color(app->renderer, colors::BLACK);
        SDL_RenderFillRect(app->renderer, &game_over_background_rect);

        TTF_Font *upheavtt = TTF_OpenFont("assets/upheavtt.ttf",
                                          gui_config::GAME_OVER_FONT_SIZE);

        text::TextStyleParams style = {
            .font = upheavtt,
            .color = colors::BORDER_WHITE,
        };

        text::draw_text_in_rect(
            text::Text("game over!", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::GAME_OVER_TEXT_X_POS,
                .relative_y_pos = gui_config::GAME_OVER_TITLE_Y_POS,
            }),
            app->renderer);

        text::draw_text_in_rect(
            text::Text("press 'r'", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::GAME_OVER_TEXT_X_POS,
                .relative_y_pos = gui_config::GAME_OVER_SUBHEADING1_Y_POS,
            }),
            app->renderer);

        text::draw_text_in_rect(
            text::Text("to try again!", style),
            game_over_background_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::GAME_OVER_TEXT_X_POS,
                .relative_y_pos = gui_config::GAME_OVER_SUBHEADING2_Y_POS,
            }),
            app->renderer);

        TTF_CloseFont(upheavtt);
    }
}