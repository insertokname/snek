#pragma once

#include <cstdint>

namespace snek {
    enum class GameState : std::uint8_t {
        Running,
        SnakeDead,
        Quitting,
    };

    class GameContext {
    public:
        [[__nodiscard__]] GameState get_cur_game_state() const;
        void set_cur_game_state(GameState new_game_state);
        GameContext() = default;

    private:
        GameState m_cur_game_state = GameState::Running;
    };
}