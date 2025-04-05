#include "game_context.hpp"

namespace snek {
    GameState GameContext::get_cur_game_state() const {
        return this->m_cur_game_state;
    }

    void GameContext::set_cur_game_state(GameState new_game_state) {
        this->m_cur_game_state = new_game_state;
    }

    std::filesystem::path GameContext::get_exe_path() const {
        return this->m_exe_path;
    }
}