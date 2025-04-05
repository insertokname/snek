#include "path.hpp"

#include <memory>

#include "dimensions.hpp"
#include "game_context.hpp"

namespace snek {
    Path::Path(const Dimensions& board_size,
               const std::shared_ptr<GameContext>& game_context)
        : m_dimensions(board_size) {
        m_path = std::vector<std::vector<std::size_t>>(
            board_size.height, std::vector<std::size_t>(board_size.width));

        if (board_size.width % 2 == 0) {
            this->m_generate_horizontal_path();
        } else if (board_size.height % 2 == 0) {
            this->m_generate_vertical_path();
        } else {
            std::string error_message =
                "Both sides of the board are odd!\n"
                "can't generate hamiltonian cycle!\n"
                "change the size of the board so that\n"
                "it contains at least one even side!\n";
            game_context->set_cur_game_state(GameState::Error);
            game_context->set_error_message(std::move(error_message));
            return;
        }
    }

    void Path::m_generate_horizontal_path() {
        for (int i = (int)(this->m_dimensions.height) - 1; i >= 0; i--) {
            m_path[i][0] = this->m_path_length++;
        }
        for (int j = 1; j < this->m_dimensions.width; j++) {
            m_path[0][j] = this->m_path_length++;
        }

        int j = (int)(this->m_dimensions.width) - 1;
        int i = 1;
        while (j > 0) {
            if (i == 1) {
                for (; i < this->m_dimensions.height; i++) {
                    m_path[i][j] = this->m_path_length++;
                }
                i = (int)(this->m_dimensions.height) - 1;
            } else {
                for (; i >= 1; i--) {
                    m_path[i][j] = this->m_path_length++;
                }
                i = 1;
            }
            j--;
        }
    }

    void Path::m_generate_vertical_path() {
        for (int j = (int)(this->m_dimensions.width) - 1; j >= 0; j--) {
            m_path[this->m_dimensions.height - 1][j] = this->m_path_length++;
        }
        for (int i = (int)(this->m_dimensions.height) - 2; i >= 0; i--) {
            m_path[i][0] = this->m_path_length++;
        }

        int j = 1;
        int i = 0;
        while (i < (int)(this->m_dimensions.height) - 1) {
            if (j == 1) {
                for (; j < this->m_dimensions.width; j++) {
                    m_path[i][j] = this->m_path_length++;
                }
                j = (int)(this->m_dimensions.width) - 1;
            } else {
                for (; j >= 1; j--) {
                    m_path[i][j] = this->m_path_length++;
                }
                j = 1;
            }
            i++;
        }
    }
}