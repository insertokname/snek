#include "SDL_main.h"
#include "app.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    snek::App app;
    app.run();
    return 0;
}