#include "board.hpp"

void snek::Board::_draw_cell(snek::App *app, std::size_t y, std::size_t x) {
    SDL_Rect rect;
    rect.x = x * CELL_SIZE;
    rect.y = y * CELL_SIZE;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;

    switch (this->_mat[y][x]) {
        using snek::Board;

        case Cell::empty:
            break;

        case Cell::tail:
            SDL_SetRenderDrawColor(app->renderer, 121, 116, 14, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case Cell::body:
            SDL_SetRenderDrawColor(app->renderer, 152, 151, 26, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case Cell::head:
            SDL_SetRenderDrawColor(app->renderer, 184, 187, 38, 255);
            SDL_RenderFillRect(app->renderer, &rect);
            break;

        case Cell::food:
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

void snek::Board::_spawn_food() {
    std::vector<std::pair<std::size_t, std::size_t>> valid_spaces;
    for (std::size_t i = 0;i < this->_height;i++) {
        for (std::size_t j = 0;j < this->_width;j++) {
            if (this->_mat[i][j] == snek::Board::Cell::empty) {
                valid_spaces.push_back(std::pair<std::size_t, std::size_t>(i, j));
            }
        }
    }

    if (valid_spaces.size() == 0) {
        std::cout << "No more spaces left to spawn food!\nYou probably won!\n";
        exit(0);
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::size_t index = std::rand() % valid_spaces.size();
    this->_mat[valid_spaces[index].first][valid_spaces[index].second] =
        snek::Board::Cell::food;
}

snek::Board::Board(std::size_t height, std::size_t width) : _width(width), _height(height) {
    this->_mat = std::vector <std::vector<Board::Cell>>(height, std::vector<Board::Cell>(width, Board::Cell::empty));

    this->_snake.push_front(std::pair<std::size_t, std::size_t>(height / 2 - 1, width / 2 - 1));
    this->_snake.push_front(std::pair<std::size_t, std::size_t>(height / 2 - 1, width / 2));

    this->_mat[height / 2 - 1][width / 2 - 1] = snek::Board::Cell::tail;
    this->_mat[height / 2 - 1][width / 2] = snek::Board::Cell::head;

    this->_spawn_food();
}

void snek::Board::draw_board(snek::App *app) {
    for (std::size_t i = 0;i < _height;i++) {
        for (std::size_t j = 0;j < _width;j++) {
            this->_draw_cell(app, i, j);
        }
    }
}

int snek::Board::move_snake(std::pair<int, int> direction) {
    if (!(direction.first || direction.second)) {
        return 0;
    }
    std::pair<int, int> new_head = this->_snake.front(),
        head_copy = this->_snake.front(),
        tail_copy = this->_snake.back(),
        second_segment = *this->_snake.begin().operator++();
    new_head.first += direction.first;
    new_head.second += direction.second;

    //if the snake exits == dead
    if (!(0 <= new_head.first && new_head.first < this->_height &&
        0 <= new_head.second && new_head.second < this->_width)) {
        return 1;
    }

    //the snake moved the oposite way and should keep course
    if (new_head == second_segment) {
        return 2;
    }

    //if the snake finds a fruit extend it
    if (this->_mat[new_head.first][new_head.second] == snek::Board::Cell::food) {
        this->_spawn_food();

        this->_snake.push_front(new_head);
        this->_mat[head_copy.first][head_copy.second] = snek::Board::Cell::body;
        this->_mat[new_head.first][new_head.second] = snek::Board::Cell::head;
        return 0;
    }
    //if the snake colides with the body == dead
    else if (this->_mat[new_head.first][new_head.second] != snek::Board::Cell::empty) {
        return 1;
    }

    //delete old tail, add new head, update old head to body
    this->_mat[head_copy.first][head_copy.second] = snek::Board::Cell::body;
    this->_mat[new_head.first][new_head.second] = snek::Board::Cell::head;
    this->_mat[tail_copy.first][tail_copy.second] = snek::Board::Cell::empty;

    //add new tail
    this->_snake.pop_back();
    this->_mat[this->_snake.back().first][this->_snake.back().second] = snek::Board::Cell::tail;
    this->_snake.push_front(new_head);
    return 0;
}