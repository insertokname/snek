#pragma once

#include "dimensions.hpp"
#include "game_context.hpp"
#include "game_loop.hpp"
#include "video_context.hpp"

namespace snek {
    class App {
    public:
        App(const Dimensions &board_size,
            const VideoContext &video_context,
            std::shared_ptr<GameContext> game_context)
            : m_game_context(std::move(game_context)),
              m_game_loop(board_size, video_context, this->m_game_context) {}
        void run();

    private:
        std::shared_ptr<GameContext> m_game_context;
        GameLoop m_game_loop;
    };
}