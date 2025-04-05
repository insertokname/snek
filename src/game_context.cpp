#include "game_context.hpp"

#include <string>

namespace snek {
    GameState GameContext::get_cur_game_state() const {
        return this->m_cur_game_state;
    }

    void GameContext::set_cur_game_state(GameState new_game_state) {
        this->m_cur_game_state = new_game_state;
    }

    std::string_view GameContext::get_error_message() const {
        return this->m_error_message;
    }

    void GameContext::set_error_message(std::string new_error_message) {
        this->m_error_message = std::move(new_error_message);
    }

    std::filesystem::path GameContext::get_exe_path() const {
        return this->m_exe_path;
    }
}