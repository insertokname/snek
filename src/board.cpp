#include "board.hpp"


enum Cell { empty, tail, body, head };

void snek::Board::_draw_cell(snek::App *app, std::size_t y, std::size_t x) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;

    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(app->renderer, &rect);
}

void snek::Board::draw_board(snek::App *app) {
    for (int i = 0;i < 5;i++) {
        for (int j = 0;j < 5;j++) {
            this->_draw_cell(app, i*CELL_SIZE, j*CELL_SIZE);
        }
    }
}
