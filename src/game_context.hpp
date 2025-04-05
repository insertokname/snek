#pragma once

#include <cstdint>
#include <filesystem>

namespace snek {
    enum class GameState : std::uint8_t {
        Running,
        SnakeDead,
        Quitting,
        Won,
    };

    class GameContext {
    public:
        [[__nodiscard__]] GameState get_cur_game_state() const;
        void set_cur_game_state(GameState new_game_state);

        [[__nodiscard__]] std::filesystem::path get_exe_path() const;

        explicit GameContext(std::filesystem::path exe_path)
            : m_exe_path(std::move(exe_path)) {}

    private:
        GameState m_cur_game_state = GameState::Running;
        std::filesystem::path m_exe_path;
    };
}