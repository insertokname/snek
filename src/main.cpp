#include <stdlib.h>
#include <iostream>
#include "SDL2/SDL.h"
#include "defs.hpp"
#include "init.hpp"
#include "loop.hpp"
#include "board.hpp"

int main(void)
{
    snek::Board board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH);
    snek::App *app = new snek::App;
    memset(app, 0, sizeof(snek::App));

    initSDL(app);

    while (1) {
        snek::prepareScene(app);


        std::pair<int, int> direction;
        snek::doInput(direction);

        if (direction.first || direction.second) {
            std::cout << direction.first << ' ' << direction.second << '\n';
        }

        if (board.move_snake(direction)) {
            std::cout << "YOU DIED";
            exit(0);
        }
        board.draw_board(app);

        snek::presentScene(app);

        SDL_Delay(16);
    }
    return 0;
}
