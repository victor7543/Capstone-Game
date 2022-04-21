#include <iostream>
#include "controller.h"
#include "SDL.h"
#include "piece.h"

void Controller::ChangeDirection(Piece& piece, Piece::Direction input) const {
    piece.direction = input;
    return;
}

bool Controller::HandleInput(bool& running, Piece& piece, bool &is_game_over) const {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
            return false;
        }
        else if (is_game_over && e.type == SDL_KEYDOWN) {
            return true;
        }
    }
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_UP]) {
        ChangeDirection(piece, Piece::Direction::kUp);
    }
    else if (keystates[SDL_SCANCODE_LEFT]) {
        ChangeDirection(piece, Piece::Direction::kLeft);
    }
    else if (keystates[SDL_SCANCODE_RIGHT]) {
        ChangeDirection(piece, Piece::Direction::kRight);
    }
    else if (keystates[SDL_SCANCODE_DOWN]) {
        ChangeDirection(piece, Piece::Direction::kDown);
    }
    else {
        ChangeDirection(piece, Piece::Direction::kNull);
    }
    return false;
}