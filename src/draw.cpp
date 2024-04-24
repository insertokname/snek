#include "draw.hpp"

void snek::draw_cell(snek::App *app, const snek::Board board, std::size_t y, std::size_t x) {
    SDL_Rect rect;
    rect.x = x * CELL_SIZE;
    rect.y = y * CELL_SIZE;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;

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

void snek::draw_board(snek::App *app, const snek::Board board) {
    for (std::size_t i = 0;i < board.height();i++) {
        for (std::size_t j = 0;j < board.width();j++) {
            snek::draw_cell(app, board, i, j);
        }
    }
}