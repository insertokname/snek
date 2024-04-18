#include "board.hpp"



void snek::Board::_draw_cell(snek::App *app, std::size_t y, std::size_t x) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;

    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(app->renderer, &rect);
}

snek::Board::Board(std::size_t height, std::size_t width) : _width(width), _height(height) {
    this->mat = std::vector <std::vector<Board::Cell>>(height, std::vector<Board::Cell>(width, Board::Cell::empty));
}

void snek::Board::draw_board(snek::App *app) {
    for (std::size_t i = 0;i < _height;i++) {
        for (std::size_t j = 0;j < _width;j++) {
            this->_draw_cell(app, i*CELL_SIZE, j*CELL_SIZE);
        }
    }
}
