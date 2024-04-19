#include <stdlib.h>
#include <iostream>
#include <chrono>
#include "SDL2/SDL.h"
#include "defs.hpp"
#include "init.hpp"
#include "loop.hpp"
#include "board.hpp"

int main(void) {
    snek::Board board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH);
    snek::App *app = new snek::App;
    memset(app, 0, sizeof(snek::App));

    initSDL(app);

    auto start = std::chrono::steady_clock::now();
    
    while (1) {
        snek::prepareScene(app);

        std::pair<int, int> raw_input;
        snek::doInput(raw_input);

        if ((std::chrono::steady_clock::now() - start) >= std::chrono::milliseconds(snek::MOVE_SPEED)) {
            start = std::chrono::steady_clock::now();
        }
        
        board.draw_board(app);

        snek::presentScene(app);

        SDL_Delay(5);
    }
    return 0;
}
