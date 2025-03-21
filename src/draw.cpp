#include "draw.hpp"

#include <climits>

void snek::draw::draw_cell(snek::App *app, const snek::Board board,
                           std::size_t y, std::size_t x) {
    int width, height;
    SDL_GetWindowSize(app->window, &width, &height);
    width -= snek::PADDING * 2;
    height -= snek::PADDING * 2;
    int cell_width = width / snek::BOARD_WIDTH;
    int cell_height = height / snek::BOARD_HEIGHT;
    int cell_size = std::min(cell_height, cell_width);

    double width_start_offset =
        (width - (cell_size * snek::BOARD_WIDTH)) / 2.0 +
        snek::PADDING;
    double height_start_offset =
        (height - (cell_size * snek::BOARD_HEIGHT)) / 2.0 +
        snek::PADDING;

    SDL_Rect rect;
    rect.x = x * cell_size + width_start_offset;
    rect.y = y * cell_size + height_start_offset;
    rect.w = cell_size;
    rect.h = cell_size;

    switch (board.mat()[y][x]) {
        using snek::Board;

        case snek::Cell::empty:
            break;

        case snek::Cell::tail:
            SDL_SetRenderDrawColor(app->renderer, 121, 116, 14, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case snek::Cell::body:
            SDL_SetRenderDrawColor(app->renderer, 152, 151, 26, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case snek::Cell::head:
            SDL_SetRenderDrawColor(app->renderer, 184, 187, 38, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case snek::Cell::food:
            SDL_SetRenderDrawColor(app->renderer, 251, 73, 52, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        default:
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;
    }
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(app->renderer, &rect);
}

void snek::draw::draw_board(snek::App *app, const snek::Board board) {
    for (std::size_t i = 0; i < board.height(); i++) {
        for (std::size_t j = 0; j < board.width(); j++) {
            snek::draw::draw_cell(app, board, i, j);
        }
    }
}