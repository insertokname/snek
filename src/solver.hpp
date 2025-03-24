#pragma once

#include "board.hpp"
#include "optional"

namespace snek {
    class Solver {
    public:
        Solver(const Board& board, std::size_t height,
               std::size_t width);
        std::pair<int, int> get_next_move();

    private:
        const Board& _board;
        std::vector<std::vector<std::size_t>> _path;
        std::size_t _path_length;
        std::size_t get_dist_to_food(
            std::pair<std::size_t, std::size_t> start_pos);
        std::size_t get_dist_to_tail(
            std::pair<std::size_t, std::size_t> start_pos);
        std::size_t get_dist_to_point(
            std::pair<std::size_t, std::size_t> start_pos,
            std::pair<std::size_t, std::size_t> end_pos);
        double get_board_to_snake_ratio();
    };
}