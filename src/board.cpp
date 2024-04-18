#include "board.hpp"



void snek::Board::_draw_cell(snek::App *app, std::size_t y, std::size_t x) {
    SDL_Rect rect;
    rect.x = x * CELL_SIZE;
    rect.y = y * CELL_SIZE;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;

    switch (this->mat[y][x]) {
        using snek::Board;

        case Cell::empty:
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(app->renderer, &rect);
            break;

        case Cell::tail:
            SDL_SetRenderDrawColor(app->renderer, 121, 116, 14, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case Cell::body:
            SDL_SetRenderDrawColor(app->renderer, 152, 151, 26, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        default:
            SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;
    }

}

snek::Board::Board(std::size_t height, std::size_t width) : _width(width), _height(height) {
    this->mat = std::vector <std::vector<Board::Cell>>(height, std::vector<Board::Cell>(width, Board::Cell::empty));
    this->mat[1][1] = snek::Board::Cell::body;
    this->mat[1][0] = snek::Board::Cell::body;
    this->mat[1][2] = snek::Board::Cell::tail;
}

void snek::Board::draw_board(snek::App *app) {
    for (std::size_t i = 0;i < _height;i++) {
        for (std::size_t j = 0;j < _width;j++) {
            this->_draw_cell(app, i, j);
        }
    }
}
