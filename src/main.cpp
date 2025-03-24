#include <stdlib.h>

#include "SDL.h"
#include "app.hpp"

int main(int argc, char *argv[]) {
#ifdef SNEK_ALGORITHM
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
#ifdef __EMSCRIPTEN__
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#document");
#endif
#endif
    snek::App app;
    app.run();
    return 0;
}
