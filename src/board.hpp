#pragma once

#include <cstdlib>
#include <ctime>
#include <deque>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "cell.hpp"
#include "dimensions.hpp"
#include "game_context.hpp"

namespace snek {
    class Board {
    public:
        explicit Board(const Dimensions& board_size);

        void move_snake(const std::shared_ptr<GameContext>& game_context);

        [[nodiscard]] Dimensions get_size() const;
        [[nodiscard]] const std::pair<std::size_t, std::size_t>&
        get_food_location() const;
        [[nodiscard]] const std::vector<std::vector<snek::Cell>>& mat() const;
        [[nodiscard]] const std::deque<std::pair<std::size_t, std::size_t>>&
        get_snake() const;

        void set_direction(std::pair<int, int> new_direction);

    private:
        std::pair<int, int> m_direction;
        Dimensions m_board_size;
        std::deque<std::pair<std::size_t, std::size_t>> m_snake;
        std::vector<std::vector<snek::Cell>> m_mat;
        std::pair<std::size_t, std::size_t> m_food_location;
        void m_spawn_food(
            std::optional<std::shared_ptr<GameContext>> game_context);
    };
}