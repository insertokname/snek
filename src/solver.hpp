#pragma once

#include <memory>
#include <optional>

#include "board.hpp"
#include "game_context.hpp"
#include "path.hpp"

namespace snek {
    class Solver {
    public:
        Solver(const std::shared_ptr<Board> &board,
               const std::shared_ptr<GameContext> &game_context)
            : m_path(board->get_size(), game_context),
              m_board(board),
              m_game_context(game_context) {}

        std::optional<std::pair<int, int>> get_next_move();

    private:
        Path m_path;
        std::shared_ptr<Board> m_board;
        std::shared_ptr<GameContext> m_game_context;
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