#include <iostream>
#include "controller.h"
#include "SDL.h"
#include "shape.h"

void Controller::ChangeDirection(Shape& shape, Shape::Direction input) const {
    shape.direction = input;
    return;
}

void Controller::HandleInput(bool& running, Shape& shape) const {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
    }
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_UP]) {
        ChangeDirection(shape, Shape::Direction::kUp);
    }
    else if (keystates[SDL_SCANCODE_LEFT]) {
        ChangeDirection(shape, Shape::Direction::kLeft);
    }
    else if (keystates[SDL_SCANCODE_RIGHT]) {
        ChangeDirection(shape, Shape::Direction::kRight);
    }
    else if (keystates[SDL_SCANCODE_DOWN]) {
        ChangeDirection(shape, Shape::Direction::kDown);
    }
    else {
        ChangeDirection(shape, Shape::Direction::kNeutral);
    }
}