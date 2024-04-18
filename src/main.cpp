#include <stdlib.h>
#include <iostream>
#include "SDL2/SDL.h"
#include "defs.hpp"
#include "init.hpp"
#include "loop.hpp"
#include "board.hpp"

int main(void)
{
    snek::Board board(10,10);
    snek::App *app = new snek::App;
    memset(app, 0, sizeof(snek::App));

    initSDL(app);

    while (1) {
        snek::prepareScene(app);

        snek::doInput();

        board.draw_board(app);

        snek::presentScene(app);

        SDL_Delay(16);
    }
    return 0;
}
