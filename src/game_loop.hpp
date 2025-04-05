#pragma once

#include <memory>
#include <utility>

#include "board.hpp"
#include "dimensions.hpp"
#include "game_context.hpp"
#include "video_context.hpp"

#ifdef SNEK_ALGORITHM
#include "solver.hpp"
#endif

namespace snek {
    class GameLoop {
    public:
        void game_tick();
        void do_input(std::pair<int, int>& direction);
        GameLoop(const Dimensions& dimensions,
                 const VideoContext& video_context,
                 std::shared_ptr<GameContext> game_context)
            : m_board(dimensions),
#ifdef SNEK_ALGORITHM
              m_solver(std::unique_ptr<Board>(&this->m_board)),
#endif
              m_video_context(video_context),
              m_dimensions(dimensions),
              m_game_context(std::move(game_context)) {
        }

    private:
        void m_prepare_scene() const;
        void m_present_scene() const;
        bool m_is_buffering = false;
        Dimensions m_dimensions;
        VideoContext m_video_context;
        std::shared_ptr<GameContext> m_game_context;
        Board m_board;
#ifdef SNEK_ALGORITHM
        Solver m_solver;
#endif
    };
}