#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <utility>
#include <vector>

#include "cell.hpp"
#include "dimensions.hpp"

namespace snek {
    enum class MoveResultSnakeStatus : std::uint8_t { Dead, Alive };

    class Board {
    public:
        explicit Board(const Dimensions& board_size);

        MoveResultSnakeStatus move_snake();

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
        void m_spawn_food();
    };
}