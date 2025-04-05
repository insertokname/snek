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
                                         .border_thickness = 4,
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