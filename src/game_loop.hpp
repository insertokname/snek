#pragma once

#include <utility>

#include "board.hpp"
#include "video_context.hpp"
#include "dimensions.hpp"

#ifdef SNEK_ALGORITHM
#include <memory>

#include "solver.hpp"
#endif

namespace snek {
    enum class GameState : std::uint8_t {
        Running,
        SnakeDead,
        Quitting,
    };

    class GameLoop {
    public:
        void game_tick();
        void do_input(std::pair<int, int>& direction);
        [[nodiscard]] GameState get_cur_game_state();
        explicit GameLoop(const Dimensions& dimensions, const VideoContext& video_context)
            : m_board(dimensions),
#ifdef SNEK_ALGORITHM
              m_solver(std::unique_ptr<Board>(&this->m_board)),
#endif
              m_context(video_context),
              m_dimensions(dimensions) {
        }

    private:
        void m_prepare_scene() const;
        void m_present_scene() const;
        bool m_is_buffering = false;
        Dimensions m_dimensions;
        VideoContext m_context;
        GameState m_cur_game_state = GameState::Running;
        Board m_board;
#ifdef SNEK_ALGORITHM
        Solver m_solver;
#endif
    };
}