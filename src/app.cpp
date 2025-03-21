#include "app.hpp"

#include <chrono>

#include "draw.hpp"

snek::App::App() : _board(snek::BOARD_HEIGHT, snek::BOARD_WIDTH) {
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't initialize SDL: " << SDL_GetError()
                  << "\n";
        exit(1);
    }

    this->window =
        SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, snek::SCREEN_WIDTH,
                         snek::SCREEN_HEIGHT, windowFlags);

    if (!this->window) {
        std::cout << "Failed to open " << snek::SCREEN_WIDTH << " x "
                  << snek::SCREEN_HEIGHT
                  << " window: " << SDL_GetError() << "\n";
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    this->renderer =
        SDL_CreateRenderer(this->window, -1, rendererFlags);

    if (!this->renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError()
                  << "\n";
        exit(1);
    }
}

void snek::App::run() {
    auto start = std::chrono::steady_clock::now();
    std::pair<int, int> direction;
    bool buffering = 0;

    while (1) {
        this->prepareScene();

        std::pair<int, int> raw_input;
        this->doInput(raw_input);

        if (!buffering && (raw_input.first || raw_input.second)) {
            direction = raw_input;
            buffering = 1;
        }

        if ((std::chrono::steady_clock::now() - start) >=
            std::chrono::microseconds(snek::MOVE_SPEED)) {
            start = std::chrono::steady_clock::now();
            buffering = 0;
            switch (this->_board.move_snake(direction)) {
                case 0:
                    break;
                case 1:
                    std::cout << "you died!";
                    exit(0);
                    break;
                case 2:
                    if (this->_board.move_snake(std::pair<int, int>(
                            direction.first * -1,
                            direction.second * -1))) {
                        std::cout << "you died!";
                        exit(0);
                    }
                    break;
                default:
                    break;
            }
        }

        snek::draw::draw_board(this, this->_board);
        this->presentScene();
        SDL_Delay(5);
    }
}

void snek::App::prepareScene() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->renderer);
}

void snek::App::presentScene() {
    SDL_RenderPresent(this->renderer);
}

void snek::App::doInput(std::pair<int, int> &direction) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_UP) {
                    direction = std::pair<int, int>(-1, 0);
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    direction = std::pair<int, int>(1, 0);
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    direction = std::pair<int, int>(0, -1);
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    direction = std::pair<int, int>(0, 1);
                }
            }
            default:
                break;
        }
    }
}