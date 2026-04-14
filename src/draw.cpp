#include "draw.hpp"

#include <SDL_pixels.h>
#include <SDL_surface.h>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

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

    struct BoardRenderLayout {
        SDL_Rect board_rect;
        int cell_size;
    };

    BoardRenderLayout get_board_layout(const VideoContext &video_context,
                                       const Dimensions &board_size) {
        SDL_Rect screen_rect;
        rect_tools::get_screen_rect(video_context.window, screen_rect);

        SDL_Rect out_game_board_rect;
        rect_tools::apply_rect_style(out_game_board_rect,
                                     screen_rect,
                                     rect_tools::SymmetrictPaddingRectStyle(
                                         {.x_padding = game_config::PADDING,
                                          .y_padding = game_config::PADDING}));

        const int cell_width =
            out_game_board_rect.w / static_cast<int>(board_size.width);
        const int cell_height =
            out_game_board_rect.h / static_cast<int>(board_size.height);
        const int cell_size = std::max(gui_config::MIN_CELL_SIZE,
                                       std::min(cell_height, cell_width));

        out_game_board_rect.w = cell_size * static_cast<int>(board_size.width);
        out_game_board_rect.h = cell_size * static_cast<int>(board_size.height);

        rect_tools::apply_rect_style(
            out_game_board_rect,
            screen_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::BOARD_XY_POS,
                .relative_y_pos = gui_config::BOARD_XY_POS,
            }));

        return {
            .board_rect = out_game_board_rect,
            .cell_size = cell_size,
        };
    }

    SDL_Rect get_cell_rect(const BoardRenderLayout &layout,
                           std::size_t cell_y_pos,
                           std::size_t cell_x_pos) {
        return {
            .x = layout.board_rect.x +
                 static_cast<int>(cell_x_pos * layout.cell_size),
            .y = layout.board_rect.y +
                 static_cast<int>(cell_y_pos * layout.cell_size),
            .w = layout.cell_size,
            .h = layout.cell_size,
        };
    }

    void draw_board_grid(const VideoContext &video_context,
                         const BoardRenderLayout &layout,
                         const Dimensions &board_size) {
        set_draw_color(video_context.renderer, colors::BORDER_WHITE);

        const int x_start = layout.board_rect.x;
        const int y_start = layout.board_rect.y;
        const int x_end = layout.board_rect.x + layout.board_rect.w;
        const int y_end = layout.board_rect.y + layout.board_rect.h;

        for (std::size_t row = 0; row <= board_size.height; row++) {
            const int row_pixel =
                y_start + static_cast<int>(row * layout.cell_size);
            SDL_RenderDrawLine(
                video_context.renderer, x_start, row_pixel, x_end, row_pixel);
        }

        for (std::size_t col = 0; col <= board_size.width; col++) {
            const int col_pixel =
                x_start + static_cast<int>(col * layout.cell_size);
            SDL_RenderDrawLine(
                video_context.renderer, col_pixel, y_start, col_pixel, y_end);
        }
    }

    void draw_cell_fill_batch(const VideoContext &video_context,
                              const std::vector<SDL_Rect> &rects,
                              colors::Color color) {
        if (rects.empty()) {
            return;
        }

        set_draw_color(video_context.renderer, color);
        SDL_RenderFillRects(video_context.renderer,
                            rects.data(),
                            static_cast<int>(rects.size()));
    }

    void set_draw_color(SDL_Renderer *renderer, colors::Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    void draw_cell(const VideoContext &video_context,
                   const std::shared_ptr<Board> &board,
                   std::size_t cell_y_pos,
                   std::size_t cell_x_pos) {
        const BoardRenderLayout layout =
            get_board_layout(video_context, board->get_size());
        const SDL_Rect cell_rect =
            get_cell_rect(layout, cell_y_pos, cell_x_pos);

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
        const Dimensions board_size = board->get_size();
        const BoardRenderLayout layout =
            get_board_layout(video_context, board_size);

        const auto &mat = board->mat();
        const std::size_t total_cells = board_size.width * board_size.height;

        std::vector<SDL_Rect> tail_cells;
        std::vector<SDL_Rect> body_cells;
        std::vector<SDL_Rect> head_cells;
        std::vector<SDL_Rect> food_cells;

        tail_cells.reserve(total_cells / 4);
        body_cells.reserve(total_cells / 4);
        head_cells.reserve(1);
        food_cells.reserve(1);

        for (std::size_t row = 0; row < board_size.height; row++) {
            for (std::size_t col = 0; col < board_size.width; col++) {
                const Cell current_cell = mat[row][col];
                if (current_cell == Cell::Empty) {
                    continue;
                }

                SDL_Rect cell_rect = get_cell_rect(layout, row, col);

                switch (current_cell) {
                    case Cell::Tail:
                        tail_cells.push_back(cell_rect);
                        break;
                    case Cell::Body:
                        body_cells.push_back(cell_rect);
                        break;
                    case Cell::Head:
                        head_cells.push_back(cell_rect);
                        break;
                    case Cell::Food:
                        food_cells.push_back(cell_rect);
                        break;
                    default:
                        break;
                }
            }
        }

        draw_cell_fill_batch(video_context, tail_cells, colors::TAIL_GREEN);
        draw_cell_fill_batch(video_context, body_cells, colors::BODY_GREEN);
        draw_cell_fill_batch(video_context, head_cells, colors::HEAD_GREEN);
        draw_cell_fill_batch(video_context, food_cells, colors::FOOD_RED);

        draw_board_grid(video_context, layout, board_size);
    }

    void draw_popup_screen_background(SDL_Rect &popup_background_screen,
                                      const SDL_Rect &screen_rect,
                                      const VideoContext &video_context) {
        rect_tools::apply_rect_style(
            popup_background_screen,
            screen_rect,
            rect_tools::SymmetricRelativeRectStyle({
                .pos_percent = gui_config::POPUP_SCREEN_XY_POS,
                .size_percent = gui_config::POPUP_SCREEN_XY_POS,
            }));

        SDL_Rect game_over_border_background_rect;
        rect_tools::apply_rect_style(game_over_border_background_rect,
                                     popup_background_screen,
                                     rect_tools::BorderRectStyle({
                                         .border_thickness =
                                             gui_config::POPUP_BORDER_THICKNESS,
                                     }));

        set_draw_color(video_context.renderer, colors::BORDER_WHITE);
        SDL_RenderFillRect(video_context.renderer,
                           &game_over_border_background_rect);

        set_draw_color(video_context.renderer, colors::BLACK);
        SDL_RenderFillRect(video_context.renderer, &popup_background_screen);
    }

    void draw_text_to_popup_screen(
        text::Text title_text,
        const std::vector<text::Text> &sub_titles_text,
        const VideoContext &video_context) {
        SDL_Rect screen_rect;
        rect_tools::get_screen_rect(video_context.window, screen_rect);

        SDL_Rect popup_screen_rect;
        draw_popup_screen_background(
            popup_screen_rect, screen_rect, video_context);

        SDL_Rect top_screen_rect;
        rect_tools::apply_rect_style(
            top_screen_rect,
            popup_screen_rect,
            rect_tools::RelativeRectStyle({
                .relative_x_pos = 0.0,
                .relative_y_pos = gui_config::POPUP_SCREEN_TOP_SIDE_Y_POS,
                .relative_width = 1.0,
                .relative_height = gui_config::POPUP_SCREEN_TOP_SIDE_HEIGHT,
            }));

        SDL_Rect bottom_screen_rect;
        rect_tools::apply_rect_style(
            bottom_screen_rect,
            popup_screen_rect,
            rect_tools::RelativeRectStyle({
                .relative_x_pos = 0.0,
                .relative_y_pos = gui_config::POPUP_SCREEN_BOTTOM_SIDE_Y_POS,
                .relative_width = 1.0,
                .relative_height = gui_config::POPUP_SCREEN_BOTTOM_SIDE_HEIGHT,
            }));

        SDL_Rect title_text_rect_container;
        text::draw_text_in_rect(
            title_text,
            top_screen_rect,
            rect_tools::RelativePositionedRectStyle({
                .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                .relative_y_pos = gui_config::POPUP_SCREEN_TITLE_Y_POS,
            }),
            video_context.renderer);

        int rect_height =
            static_cast<int>(screen_rect.h / sub_titles_text.size());
        double sub_title_relative_height =
            (double)(screen_rect.h) /
            (double)(sub_titles_text.size() * screen_rect.h);

        int cur_sub_title = 0;
        for (text::Text sub_title_text : sub_titles_text) {
            SDL_Rect text_rect_container;
            rect_tools::apply_rect_style(
                text_rect_container,
                bottom_screen_rect,
                rect_tools::RelativeRectStyle({
                    .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                    .relative_y_pos = sub_title_relative_height /
                                      (1.0 - sub_title_relative_height) *
                                      cur_sub_title,
                    .relative_width = 1.0,
                    .relative_height = sub_title_relative_height,
                }));

            SDL_Rect text_rect;
            text::draw_text_in_rect(
                sub_title_text,
                text_rect_container,
                rect_tools::RelativePositionedRectStyle({
                    .relative_x_pos = gui_config::POPUP_SCREEN_TEXT_X_POS,
                    .relative_y_pos = gui_config::POPUP_SCREEN_SUB_TITLE_Y_POS,
                }),
                video_context.renderer);

            text_rect.y = rect_height * cur_sub_title;

            cur_sub_title++;
        }
    }

    struct DrawStringExtraOptions {
        colors::Color title_color = colors::BORDER_WHITE;
        int title_size = gui_config::POPUP_SCREEN_TITLE_FONT_SIZE;
        colors::Color sub_title_color = colors::BORDER_WHITE;
        int sub_title_size = gui_config::POPUP_SCREEN_SUB_TITLE_FONT_SIZE;
    };

    void draw_string_to_popup_screen(
        const std::string_view title,
        const std::vector<std::string_view> &sub_titles,
        const VideoContext &video_context,
        const std::shared_ptr<GameContext> &game_context,
        const DrawStringExtraOptions &extra_options =
            DrawStringExtraOptions()) {
        TTF_Font *title_font =
            TTF_OpenFont((game_context->get_exe_path().parent_path() /
                          "assets" / "upheavtt.ttf")
                             .string()
                             .c_str(),
                         text::get_relative_font_size(extra_options.title_size,
                                                      video_context.window));

        TTF_Font *sub_title_font = TTF_OpenFont(
            (game_context->get_exe_path().parent_path() / "assets" /
             "upheavtt.ttf")
                .string()
                .c_str(),
            text::get_relative_font_size(extra_options.sub_title_size,
                                         video_context.window));

        text::TextStyleParams title_text_style = {
            .color = extra_options.title_color,
            .font = title_font,
        };

        text::TextStyleParams sub_title_text_style = {
            .color = extra_options.sub_title_color,
            .font = sub_title_font,
        };

        std::vector<text::Text> text_sub_titles;
        text_sub_titles.reserve(sub_titles.size());

        for (const std::string_view sub_title : sub_titles) {
            text_sub_titles.emplace_back(sub_title, sub_title_text_style);
        }

        draw_text_to_popup_screen(text::Text(title, title_text_style),
                                  text_sub_titles,
                                  video_context);

        TTF_CloseFont(title_font);
        TTF_CloseFont(sub_title_font);
    }

    void draw_win_screen(const VideoContext &video_context,
                         const std::shared_ptr<GameContext> &game_context) {
        draw_string_to_popup_screen("You won!",
                                    {
                                        "you can restart",
                                        "by pressing \"r\"",
                                    },
                                    video_context,
                                    game_context);
    }

    void draw_lose_screen(const VideoContext &video_context,
                          const std::shared_ptr<GameContext> &game_context) {
        draw_string_to_popup_screen("game over!",
                                    {
                                        "press \"r\"",
                                        "to try again",
                                    },
                                    video_context,
                                    game_context);
    }
    void draw_error_screen(std::string_view error_message,
                           const VideoContext &video_context,
                           const std::shared_ptr<GameContext> &game_context) {
        std::string string_error_message(error_message);

        std::replace(string_error_message.begin(),
                     string_error_message.end(),
                     '\n',
                     '\0');

        std::vector<std::string_view> error_messages;
        error_messages.reserve(std::ranges::count(string_error_message, '\0') -
                               1);
        for (std::size_t i = 0; i < string_error_message.size();) {
            const char *start = &string_error_message[i];
            std::size_t len = strlen(start);
            if (len > 0) {
                error_messages.emplace_back(start, len);
            }
            i += len + 1;
        }

        draw_string_to_popup_screen(
            "Error:",
            error_messages,
            video_context,
            game_context,
            {
                .title_color = colors::WARNING_RED,
                .sub_title_size =
                    gui_config::POPUP_SCREEN_ERROR_SUB_TITLE_FONT_SIZE,
            });
    }
}