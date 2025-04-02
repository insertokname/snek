#include "board.hpp"

#include <cstddef>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

#include "cell.hpp"
#include "dimensions.hpp"

namespace snek {
    void Board::m_spawn_food() {
        std::vector<std::pair<std::size_t, std::size_t>> valid_spaces;
        for (std::size_t i = 0; i < this->m_board_size.height; i++) {
            for (std::size_t j = 0; j < this->m_board_size.width; j++) {
                if (this->m_mat[i][j] == Cell::Empty) {
                    valid_spaces.emplace_back(i, j);
                }
            }
        }

        if (valid_spaces.empty()) {
            std::cout << "No more spaces left to spawn Food!\nYou "
                         "probably won!\n";
            std::exit(0);
        }

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        std::size_t index = std::rand() % valid_spaces.size();
        this->m_food_location = valid_spaces[index];
        this->m_mat[valid_spaces[index].first][valid_spaces[index].second] =
            Cell::Food;
    }

    Board::Board(const Dimensions &board_size) : m_board_size(board_size) {
        this->m_mat = std::vector<std::vector<Cell>>(
            board_size.height,
            std::vector<Cell>(board_size.width, Cell::Empty));

        this->m_snake.emplace_front(0, (board_size.width / 2) - 1);
        this->m_snake.emplace_front(0, board_size.width / 2);

        this->m_mat[0][(board_size.width / 2) - 1] = Cell::Tail;
        this->m_mat[0][board_size.width / 2] = Cell::Head;

        this->m_spawn_food();
    }
    void Board::set_direction(std::pair<int, int> new_direction) {
        if (new_direction.first != 0 || new_direction.second != 0) {
            this->m_direction = new_direction;
        }
    }

    MoveResultSnakeStatus Board::move_snake() {
        if (this->m_direction.first == 0 && this->m_direction.second == 0) {
            return MoveResultSnakeStatus::Alive;
        }
        std::pair<int, int> new_head = this->m_snake.front(),
                            head_copy = this->m_snake.front(),
                            tail_copy = this->m_snake.back(),
                            second_segment =
                                *this->m_snake.begin().operator++();
        new_head.first += this->m_direction.first;
        new_head.second += this->m_direction.second;

        // the snake moved the oposite way and should keep course
        if (new_head == second_segment) {
            this->m_direction.first *= -1;
            this->m_direction.second *= -1;
            new_head = head_copy;
            new_head.first += this->m_direction.first;
            new_head.second += this->m_direction.second;
        }

        // if the snake exits == dead
        if (!(0 <= new_head.first &&
              (unsigned int)new_head.first < this->m_board_size.height &&
              0 <= new_head.second &&
              (unsigned int)new_head.second < this->m_board_size.width)) {
            return MoveResultSnakeStatus::Dead;
        }

        // if the snake finds a fruit extend it
        if (this->m_mat[new_head.first][new_head.second] == Cell::Food) {
            this->m_spawn_food();

            this->m_snake.emplace_front(new_head);
            this->m_mat[head_copy.first][head_copy.second] = Cell::Body;
            this->m_mat[new_head.first][new_head.second] = Cell::Head;
            return MoveResultSnakeStatus::Alive;
        }
        // if the snake colides with the Body == dead
        else if (this->m_mat[new_head.first][new_head.second] != Cell::Empty) {
            return MoveResultSnakeStatus::Dead;
        }

        // delete old Tail, add new Head, update old Head to Body
        this->m_mat[head_copy.first][head_copy.second] = Cell::Body;
        this->m_mat[new_head.first][new_head.second] = Cell::Head;
        this->m_mat[tail_copy.first][tail_copy.second] = Cell::Empty;

        // add new Tail
        this->m_snake.pop_back();
        this->m_mat[this->m_snake.back().first][this->m_snake.back().second] =
            Cell::Tail;
        this->m_snake.emplace_front(new_head);
        return MoveResultSnakeStatus::Alive;
    }

    Dimensions Board::get_size() const {
        return this->m_board_size;
    }
    const std::vector<std::vector<Cell>> &Board::mat() const {
        return this->m_mat;
    }

    const std::deque<std::pair<std::size_t, std::size_t>> &Board::get_snake()
        const {
        return this->m_snake;
    }

    const std::pair<std::size_t, std::size_t> &Board::get_food_location()
        const {
        return this->m_food_location;
    }
}