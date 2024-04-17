#include <stdlib.h>
#include <iostream>
#include "SDL2/SDL.h"
#include "defs.hpp"
#include "init.hpp"
#include "loop.hpp"

int main(void)
{
    snek::App *app = new snek::App;
    memset(app, 0, sizeof(snek::App));

    initSDL(app);

    while (1) {
        snek::prepareScene(app);

        snek::doInput();

        snek::presentScene(app);

        SDL_Delay(16);
    }
    return 0;
}
