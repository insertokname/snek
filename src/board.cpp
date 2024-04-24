#include "board.hpp"

void snek::Board::_spawn_food() {
    std::vector<std::pair<std::size_t, std::size_t>> valid_spaces;
    for (std::size_t i = 0;i < this->_height;i++) {
        for (std::size_t j = 0;j < this->_width;j++) {
            if (this->_mat[i][j] == snek::Cell::empty) {
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
        snek::Cell::food;
}

snek::Board::Board(std::size_t height, std::size_t width) : _width(width), _height(height) {
    this->_mat = std::vector <std::vector<Cell>>(height, std::vector<Cell>(width, Cell::empty));

    this->_snake.push_front(std::pair<std::size_t, std::size_t>(height / 2 - 1, width / 2 - 1));
    this->_snake.push_front(std::pair<std::size_t, std::size_t>(height / 2 - 1, width / 2));

    this->_mat[height / 2 - 1][width / 2 - 1] = snek::Cell::tail;
    this->_mat[height / 2 - 1][width / 2] = snek::Cell::head;

    this->_spawn_food();
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
    if (this->_mat[new_head.first][new_head.second] == snek::Cell::food) {
        this->_spawn_food();

        this->_snake.push_front(new_head);
        this->_mat[head_copy.first][head_copy.second] = snek::Cell::body;
        this->_mat[new_head.first][new_head.second] = snek::Cell::head;
        return 0;
    }
    //if the snake colides with the body == dead
    else if (this->_mat[new_head.first][new_head.second] != snek::Cell::empty) {
        return 1;
    }

    //delete old tail, add new head, update old head to body
    this->_mat[head_copy.first][head_copy.second] = snek::Cell::body;
    this->_mat[new_head.first][new_head.second] = snek::Cell::head;
    this->_mat[tail_copy.first][tail_copy.second] = snek::Cell::empty;

    //add new tail
    this->_snake.pop_back();
    this->_mat[this->_snake.back().first][this->_snake.back().second] = snek::Cell::tail;
    this->_snake.push_front(new_head);
    return 0;
}

const std::size_t snek::Board::height() const {
    return this->_height;
}

const std::size_t snek::Board::width() const {
    return this->_width;
}

const std::vector <std::vector<snek::Cell>> &snek::Board::mat() const {
    return this->_mat;
}