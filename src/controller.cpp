#include <iostream>
#include "controller.h"
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake& snake, Snake::Direction input) const {
    snake.direction = input;
    return;
}

void Controller::HandleInput(bool& running, Snake& snake) const {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
    }
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_UP]) {
        ChangeDirection(snake, Snake::Direction::kUp);
    }
    else if (keystates[SDL_SCANCODE_LEFT]) {
        ChangeDirection(snake, Snake::Direction::kLeft);
    }
    else if (keystates[SDL_SCANCODE_RIGHT]) {
        ChangeDirection(snake, Snake::Direction::kRight);
    }
    else if (keystates[SDL_SCANCODE_DOWN]) {
        ChangeDirection(snake, Snake::Direction::kDown);
    }
    else {
        ChangeDirection(snake, Snake::Direction::kNeutral);
    }
}