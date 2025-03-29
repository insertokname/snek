#include "solver.hpp"

#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "board.hpp"
#include "config.hpp"
#include "dimensions.hpp"

snek::Solver::Solver(std::unique_ptr<Board> board,
                     const Dimensions& dimensions)
    : m_board(std::move(board)), m_path(dimensions) {}

std::pair<int, int> snek::Solver::get_next_move() {
    static constexpr std::array<std::pair<int, int>, 4> DIRECTIONS = {
        std::pair{1, 0}, std::pair{0, 1}, std::pair{-1, 0},
        std::pair{0, -1}};

    std::pair<std::size_t, std::size_t> pos =
        this->m_board->get_snake().front();
    std::size_t min_dist = SIZE_MAX;
    std::pair<int, int> min_pos;

    std::size_t orig_dist_food =
        m_get_dist_to_food(std::pair(pos.first, pos.second));
    std::size_t orig_dist_tail =
        m_get_dist_to_tail(std::pair(pos.first, pos.second));
    bool can_do_shortcuts =
        (orig_dist_food < orig_dist_tail) &&
        (m_get_board_to_snake_ratio() < FOLLOW_EXACT_THRESHOLD);

    for (const auto& [ydir, xdir] : DIRECTIONS) {
        int ynew = (int)(pos.first) + ydir,
            xnew = (int)(pos.second) + xdir;

        if (((0 <= ynew && ynew < this->m_board->height()) &&
             (0 <= xnew && xnew < this->m_board->width())) &&
            (this->m_board->mat()[ynew][xnew] == snek::Cell::Empty ||
             this->m_board->mat()[ynew][xnew] == snek::Cell::Food)) {
            std::size_t dist_food =
                m_get_dist_to_food(std::pair(ynew, xnew));

            if (can_do_shortcuts ||
                ((this->m_path.get_path()[ynew][xnew] ==
                  this->m_path.get_path()[pos.first][pos.second] +
                      1) ||
                 (this->m_path.get_path()[ynew][xnew] == 0 &&
                  this->m_path.get_path()[pos.first][pos.second] ==
                      this->m_path.get_path_len() - 1))) {
                if (min_dist > dist_food) {
                    min_dist = dist_food;
                    min_pos = std::pair(ydir, xdir);
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

std::size_t snek::Solver::m_get_dist_to_point(
    std::pair<std::size_t, std::size_t> start_pos,
    std::pair<std::size_t, std::size_t> end_pos) {
    std::size_t start_val =
        this->m_path.get_path()[start_pos.first][start_pos.second];
    std::size_t end_val =
        this->m_path.get_path()[end_pos.first][end_pos.second];

    if (end_val < start_val) {
        end_val += m_path.get_path_len();
    }

    return end_val - start_val;
}

std::size_t snek::Solver::m_get_dist_to_food(
    std::pair<std::size_t, std::size_t> start_pos) {
    return this->m_get_dist_to_point(
        start_pos, this->m_board->get_food_location());
}

std::size_t snek::Solver::m_get_dist_to_tail(
    std::pair<std::size_t, std::size_t> start_pos) {
    return this->m_get_dist_to_point(
        start_pos, this->m_board->get_snake().back());
}

double snek::Solver::m_get_board_to_snake_ratio() {
    return double(this->m_board->get_snake().size()) /
           double(this->m_board->height() * this->m_board->width());
}