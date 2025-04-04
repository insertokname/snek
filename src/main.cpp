#include "app.hpp"
#include "config.hpp"
#include "video_context.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    snek::VideoContext video_context =
        snek::VideoContext(snek::game_config::INITIAL_SCREEN_SIZE);
    snek::App app(snek::game_config::BOARD_SIZE, video_context);
    app.run();
    return 0;
}