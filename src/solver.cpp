#include "solver.hpp"

#include <cstddef>

snek::Solver::Solver(const Board &board, std::size_t height,
                     std::size_t width)
    : _board(board) {
    _path = std::vector<std::vector<std::size_t>>(
        height, std::vector<std::size_t>(width));

    std::size_t index = 0;

    // we generate a hamiltonian cycle like this:
    //  * - * - * - * - * - *
    //  |                   |
    //  *   * - *   * - *   *
    //  |   |   |   |   |   |
    //  * - *   * - *   * - *
    if (width % 2 == 0) {
        for (int i = height - 1; i >= 0; i--) {
            _path[i][0] = index++;
        }
        for (int j = 1; j < width; j++) {
            _path[0][j] = index++;
        }

        int j = width - 1;
        int i = 1;
        while (j > 0) {
            if (i == 1) {
                for (; i < height; i++) {
                    _path[i][j] = index++;
                }
                i = height - 1;
            } else {
                for (; i >= 1; i--) {
                    _path[i][j] = index++;
                }
                i = 1;
            }
            j--;
        }
    }
    // generating this cycle:
    //  * - * - * - * - *
    //  |               |
    //  *   * - * - * - *
    //  |   |
    //  *   * - * - * - *
    //  |               |
    //  *   * - * - * - *
    //  |   |
    //  *   * - * - * - *
    //  |               |
    //  * - * - * - * - *
    else if (height % 2 == 0) {
        for (int j = width - 1; j >= 0; j--) {
            _path[height - 1][j] = index++;
        }
        for (int i = height - 2; i >= 0; i--) {
            _path[i][0] = index++;
        }

        int j = 1;
        int i = 0;
        while (i < height - 1) {
            if (j == 1) {
                for (; j < width; j++) {
                    _path[i][j] = index++;
                }
                j = width - 1;
            } else {
                for (; j >= 1; j--) {
                    _path[i][j] = index++;
                }
                j = 1;
            }
            i++;
        }
    } else {
        std::cout << "Both sides of the board are odd!\n"
                  << "Can't generate hamiltonian cycle!\n"
                  << "Exiting!\n";
        exit(1);
    }

    _path_length = index;
}

std::pair<int, int> snek::Solver::get_next_move() {
    int Xdir[4] = {0, 1, 0, -1}, Ydir[4] = {1, 0, -1, 0};

    std::pair<std::size_t, std::size_t> pos =
        this->_board.get_snake().front();
    std::size_t min_dist = SIZE_MAX;
    std::pair<int, int> min_pos;

    std::size_t orig_dist_food =
        get_dist_to_food(std::pair(pos.first, pos.second));
    std::size_t orig_dist_tail =
        get_dist_to_tail(std::pair(pos.first, pos.second));
    bool can_do_shortcuts = (orig_dist_food < orig_dist_tail) &&
                            (get_board_to_snake_ratio() < 0.60);

    for (int i = 0; i < 4; i++) {
        int Ynew = pos.first + Ydir[i], Xnew = pos.second + Xdir[i];

        if (((0 <= Ynew && Ynew < this->_board.height()) &&
             (0 <= Xnew && Xnew < this->_board.width())) &&
            (this->_board.mat()[Ynew][Xnew] == snek::Cell::empty ||
             this->_board.mat()[Ynew][Xnew] == snek::Cell::food)) {
            std::size_t dist_food =
                get_dist_to_food(std::pair(Ynew, Xnew));

            if (can_do_shortcuts ||
                ((this->_path[Ynew][Xnew] ==
                  this->_path[pos.first][pos.second] + 1) ||
                 (this->_path[Ynew][Xnew] == 0 &&
                  this->_path[pos.first][pos.second] ==
                      this->_path_length - 1))) {
                if (min_dist > dist_food) {
                    min_dist = dist_food;
                    min_pos = std::pair(Ydir[i], Xdir[i]);
                }
            }
        }
    }

    if (min_dist == SIZE_MAX) {
        std::cout << "Failed to reach the next cell in the path!\n"
                  << "Exiting\n";
        exit(1);
    }

    return min_pos;
}

std::size_t snek::Solver::get_dist_to_point(
    std::pair<std::size_t, std::size_t> start_pos,
    std::pair<std::size_t, std::size_t> end_pos) {
    std::size_t start_val =
        this->_path[start_pos.first][start_pos.second];
    std::size_t end_val = this->_path[end_pos.first][end_pos.second];

    if (end_val < start_val) {
        end_val += _path_length;
    }

    return end_val - start_val;
}

std::size_t snek::Solver::get_dist_to_food(
    std::pair<std::size_t, std::size_t> start_pos) {
    return this->get_dist_to_point(start_pos,
                                   this->_board.get_food_location());
}

std::size_t snek::Solver::get_dist_to_tail(
    std::pair<std::size_t, std::size_t> start_pos) {
    return this->get_dist_to_point(start_pos,
                                   this->_board.get_snake().back());
}

double snek::Solver::get_board_to_snake_ratio() {
    return double(this->_board.get_snake().size()) /
           (this->_board.height() * this->_board.width());
}