#pragma once

#include <memory>

#include "board.hpp"
#include "path.hpp"

namespace snek {
    class Solver {
    public:
        explicit Solver(std::unique_ptr<Board> board);
        std::pair<int, int> get_next_move();

    private:
        Path m_path;
        std::unique_ptr<Board> m_board;
        [[nodiscard]] std::size_t m_get_dist_to_food(
            std::pair<std::size_t, std::size_t> start_pos);
        [[nodiscard]] std::size_t m_get_dist_to_tail(
            std::pair<std::size_t, std::size_t> start_pos);
        [[nodiscard]] std::size_t m_get_dist_to_point(
            std::pair<std::size_t, std::size_t> start_pos,
            std::pair<std::size_t, std::size_t> end_pos);
        [[nodiscard]] double m_get_board_to_snake_ratio();
    };
}