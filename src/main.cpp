#include <filesystem>
#include <memory>
#include <span>

#include "app.hpp"
#include "config.hpp"
#include "game_context.hpp"
#include "video_context.hpp"

int main([[maybe_unused]] int argc, char *argv[]) {
    auto args = std::span(argv, size_t(argc));

    snek::VideoContext video_context =
        snek::VideoContext(snek::game_config::INITIAL_SCREEN_SIZE);
    snek::GameContext game_context = snek::GameContext(
        std::filesystem::path(std::filesystem::canonical(args[0])));
    snek::App app(snek::game_config::BOARD_SIZE,
                  video_context,
                  std::shared_ptr<snek::GameContext>(&game_context));
    app.run();
    return 0;
}