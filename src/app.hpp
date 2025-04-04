#pragma once

#include "video_context.hpp"
#include "dimensions.hpp"
#include "game_loop.hpp"

namespace snek {
    class App {
    public:
        explicit App(const Dimensions &board_size,
                     const Dimensions &window_size)
            : m_game_loop(board_size, VideoContext(window_size)) {}

        explicit App(const Dimensions &board_size, const VideoContext &video_context)
            : m_game_loop(board_size, video_context) {}
        void run();

    private:
        GameLoop m_game_loop;
    };
}