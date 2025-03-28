#include "board.hpp"

#include <cstddef>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

#include "cell.hpp"

void snek::Board::m_spawn_food() {
    std::vector<std::pair<std::size_t, std::size_t>> valid_spaces;
    for (std::size_t i = 0; i < this->m_height; i++) {
        for (std::size_t j = 0; j < this->m_width; j++) {
            if (this->m_mat[i][j] == snek::Cell::Empty) {
                valid_spaces.emplace_back(i, j);
            }
        }
    }

    if (valid_spaces.empty()) {
        std::cout << "No more spaces left to spawn Food!\nYou "
                     "probably won!\n";
        exit(0);
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::size_t index = std::rand() % valid_spaces.size();
    this->m_food_location = valid_spaces[index];
    this->m_mat[valid_spaces[index].first]
               [valid_spaces[index].second] = snek::Cell::Food;
}

snek::Board::Board(std::size_t height, std::size_t width)
    : m_width(width), m_height(height) {
    this->m_mat = std::vector<std::vector<Cell>>(
        height, std::vector<Cell>(width, Cell::Empty));

    this->m_snake.emplace_front(0, (width / 2) - 1);
    this->m_snake.emplace_front(0, width / 2);

    this->m_mat[0][(width / 2) - 1] = snek::Cell::Tail;
    this->m_mat[0][width / 2] = snek::Cell::Head;

    this->m_spawn_food();
}

snek::SnakeStatus snek::Board::move_snake(
    std::pair<int, int> direction) {
    if (!(direction.first != 0 || direction.second != 0)) {
        return snek::SnakeStatus::Alive;
    }
    std::pair<int, int> new_head = this->m_snake.front(),
                        head_copy = this->m_snake.front(),
                        tail_copy = this->m_snake.back(),
                        second_segment =
                            *this->m_snake.begin().operator++();
    new_head.first += direction.first;
    new_head.second += direction.second;

    // the snake moved the oposite way and should keep course
    if (new_head == second_segment) {
        direction.first *= -1;
        direction.second *= -1;
        new_head = head_copy;
        new_head.first += direction.first;
        new_head.second += direction.second;
    }
    
    // if the snake exits == dead
    if (!(0 <= new_head.first &&
          (unsigned int)new_head.first < this->m_height &&
          0 <= new_head.second &&
          (unsigned int)new_head.second < this->m_width)) {
        return snek::SnakeStatus::Dead;
    }

    // if the snake finds a fruit extend it
    if (this->m_mat[new_head.first][new_head.second] ==
        snek::Cell::Food) {
        this->m_spawn_food();

        this->m_snake.emplace_front(new_head);
        this->m_mat[head_copy.first][head_copy.second] =
            snek::Cell::Body;
        this->m_mat[new_head.first][new_head.second] =
            snek::Cell::Head;
        return snek::SnakeStatus::Alive;
    }
    // if the snake colides with the Body == dead
    else if (this->m_mat[new_head.first][new_head.second] !=
             snek::Cell::Empty) {
        return snek::SnakeStatus::Dead;
    }

    // delete old Tail, add new Head, update old Head to Body
    this->m_mat[head_copy.first][head_copy.second] = snek::Cell::Body;
    this->m_mat[new_head.first][new_head.second] = snek::Cell::Head;
    this->m_mat[tail_copy.first][tail_copy.second] =
        snek::Cell::Empty;

    // add new Tail
    this->m_snake.pop_back();
    this->m_mat[this->m_snake.back().first]
               [this->m_snake.back().second] = snek::Cell::Tail;
    this->m_snake.emplace_front(new_head);
    return snek::SnakeStatus::Alive;
}

std::size_t snek::Board::height() const {
    return this->m_height;
}

std::size_t snek::Board::width() const {
    return this->m_width;
}

const std::vector<std::vector<snek::Cell>> &snek::Board::mat() const {
    return this->m_mat;
}

const std::deque<std::pair<std::size_t, std::size_t>> &
snek::Board::get_snake() const {
    return this->m_snake;
}

const std::pair<std::size_t, std::size_t> &
snek::Board::get_food_location() const {
    return this->m_food_location;
}
