#pragma once

#include "dimensions.hpp"
#include "game_context.hpp"
#include "game_loop.hpp"
#include "video_context.hpp"

namespace snek {
    class App {
    public:
        App(const Dimensions &board_size, const Dimensions &window_size)
            : m_game_loop(board_size,
                          VideoContext(window_size),
                          this->m_game_context) {}

        App(const Dimensions &board_size, const VideoContext &video_context)
            : m_game_loop(board_size, video_context, this->m_game_context) {}
        void run();

    private:
        std::shared_ptr<GameContext> m_game_context =
            std::make_shared<GameContext>();
        GameLoop m_game_loop;
    };
}