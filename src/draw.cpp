#include "draw.hpp"

#include <SDL_pixels.h>
#include <SDL_surface.h>

#include <algorithm>
#include <cstddef>

#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "app.hpp"
#include "board.hpp"
#include "cell.hpp"
#include "colors.hpp"
#include "config.hpp"

void snek::draw::set_draw_color(SDL_Renderer *renderer,
                                snek::colors::Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b,
                           color.a);
}

void snek::draw::draw_cell(snek::App *app, const snek::Board &board,
                           std::size_t y, std::size_t x) {
    int width = 0, height = 0;
    SDL_GetWindowSize(app->window, &width, &height);
    width -= snek::PADDING * 2;
    height -= snek::PADDING * 2;
    const std::size_t cell_width = width / snek::BOARD_WIDTH;
    const std::size_t cell_height = height / snek::BOARD_HEIGHT;
    const std::size_t cell_size = std::min(cell_height, cell_width);

    const double width_start_offset =
        ((width - ((double)(cell_size)*snek::BOARD_WIDTH)) / 2.0) +
        snek::PADDING;
    const double height_start_offset =
        ((height - ((double)(cell_size)*snek::BOARD_HEIGHT)) / 2.0) +
        snek::PADDING;

    SDL_Rect rect;
    rect.x = (int)x * (int)cell_size + (int)width_start_offset;
    rect.y = (int)y * (int)cell_size + (int)height_start_offset;
    rect.w = (int)cell_size;
    rect.h = (int)cell_size;

    switch (board.mat()[y][x]) {
        using snek::Board;

        case snek::Cell::Empty:
            break;

        case snek::Cell::Tail:
            set_draw_color(app->renderer, snek::colors::TAIL_GREEN);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case snek::Cell::Body:
            set_draw_color(app->renderer, snek::colors::BODY_GREEN);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case snek::Cell::Head:
            set_draw_color(app->renderer, snek::colors::HEAD_GREEN);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case snek::Cell::Food:
            set_draw_color(app->renderer, snek::colors::FOOD_RED);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        default:
            set_draw_color(app->renderer, snek::colors::BORDER_WHITE);
            SDL_RenderFillRect(app->renderer, &rect);
            break;
    }
    set_draw_color(app->renderer, snek::colors::BORDER_WHITE);
    SDL_RenderDrawRect(app->renderer, &rect);
}

void snek::draw::draw_board(snek::App *app,
                            const snek::Board &board) {
    for (std::size_t i = 0; i < board.height(); i++) {
        for (std::size_t j = 0; j < board.width(); j++) {
            snek::draw::draw_cell(app, board, i, j);
        }
    }
}

void snek::draw::draw_game_over_screen(snek::App *app) {
    set_draw_color(app->renderer, snek::colors::BODY_GREEN);

    int width = 0, height = 0;
    SDL_GetWindowSize(app->window, &width, &height);

    SDL_Rect game_over_background;
    game_over_background.x = width / 4;
    game_over_background.y = height / 4;

    game_over_background.w = width / 2;
    game_over_background.h = height / 2;

    TTF_Font *upheavtt = TTF_OpenFont("assets/upheavtt.ttf", 24);

    SDL_Surface *surface =
        TTF_RenderText_Solid(upheavtt, "test", {255, 255, 255});

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(app->renderer, surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstrect = {0, 0, texW, texH};

    SDL_RenderCopy(app->renderer, texture, nullptr, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    SDL_RenderFillRect(app->renderer, &game_over_background);
}