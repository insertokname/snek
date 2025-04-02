#include "app.hpp"
#include "config.hpp"
#include "context.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    snek::Context context = snek::Context(snek::game_config::INITIAL_SCREEN_SIZE);
    snek::App app(snek::game_config::BOARD_SIZE, context);
    app.run();
    return 0;
}