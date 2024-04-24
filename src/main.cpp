#include <stdlib.h>
#include <iostream>
#include <chrono>
#include "SDL2/SDL.h"
#include "defs.hpp"
#include "init.hpp"
#include "loop.hpp"
#include "board.hpp"
#include "draw.hpp"

int main(void) {
    snek::Board board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH);
    snek::App *app = new snek::App;
    memset(app, 0, sizeof(snek::App));

    initSDL(app);

    auto start = std::chrono::steady_clock::now();
    std::pair<int,int> direction;
    bool buffering = 0;
    
    while (1) {
        snek::prepareScene(app);

        std::pair<int, int> raw_input;
        snek::doInput(raw_input);

        if (!buffering && (raw_input.first || raw_input.second)) {
            direction = raw_input;
            buffering = 1;
        }

        if ((std::chrono::steady_clock::now() - start) >= std::chrono::milliseconds(snek::MOVE_SPEED)) {
            start = std::chrono::steady_clock::now();
            buffering = 0;
            switch (board.move_snake(direction)) {
                case 0:
                    break;
                case 1:
                    std::cout << "you died!";
                    exit(0);
                    break;
                case 2:
                    if (board.move_snake(std::pair<int, int>(direction.first * -1, direction.second * -1))) {
                        std::cout << "you died!";
                        exit(0);
                    }
                    break;
                default:
                    break;
            }
        }
        
        snek::draw_board(app,board);

        snek::presentScene(app);

        SDL_Delay(5);
    }
    return 0;
}
