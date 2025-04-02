#pragma once

#include "context.hpp"
#include "dimensions.hpp"
#include "game_loop.hpp"

namespace snek {
    class App {
    public:
        explicit App(const Dimensions &board_size,
                     const Dimensions &window_size)
            : m_game_loop(board_size, Context(window_size)) {}

        explicit App(const Dimensions &board_size, const Context &context)
            : m_game_loop(board_size, context) {}
        void run();

    private:
        GameLoop m_game_loop;
    };
}