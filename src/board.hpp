#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <utility>
#include <vector>

#include "cell.hpp"


namespace snek {
    enum class SnakeStatus : std::uint8_t { Dead, Alive };

    class Board {
    public:
        Board(std::size_t height, std::size_t width);

        SnakeStatus move_snake(std::pair<int, int> direction);

        [[nodiscard]] std::size_t height() const;
        [[nodiscard]] std::size_t width() const;
        [[nodiscard]] const std::pair<std::size_t, std::size_t>&
        get_food_location() const;
        [[nodiscard]] const std::vector<std::vector<snek::Cell>>&
        mat() const;
        [[nodiscard]] const std::deque<
            std::pair<std::size_t, std::size_t>>&
        get_snake() const;

    private:
        std::size_t m_height, m_width;
        std::deque<std::pair<std::size_t, std::size_t>> m_snake;
        std::vector<std::vector<snek::Cell>> m_mat;
        std::pair<std::size_t, std::size_t> m_food_location;
        void m_spawn_food();
    };
}