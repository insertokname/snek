#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "dimensions.hpp"
#include "game_context.hpp"

namespace snek {
    class Path {
    public:
        explicit Path(const Dimensions& board_size,
                      const std::shared_ptr<GameContext>& game_context);

        [[nodiscard]] const std::vector<std::vector<std::size_t>>& get_path()
            const {
            return this->m_path;
        }
        [[nodiscard]] const std::size_t get_path_len() const {
            return this->m_path_length;
        }

    private:
        Dimensions m_dimensions;
        std::size_t m_path_length = 0;
        std::vector<std::vector<std::size_t>> m_path;

        void m_generate_horizontal_path();
        void m_generate_vertical_path();
    };
}