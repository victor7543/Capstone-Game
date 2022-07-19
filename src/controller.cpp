#include <iostream>
#include "controller.h"
#include "SDL.h"
#include "piece.h"

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
        if (rotate_key_already_pressed) {
            piece.rotate_key_held = true;
        }
        rotate_key_already_pressed = true;
    }
    else {
        piece.rotated = false;
        piece.rotate_key_held = false;
        rotate_key_already_pressed = false;
    }
    if (keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT]) {
        piece.horiz_direction = Piece::Direction::kNull;
        if (horiz_key_already_pressed) {
            piece.horiz_key_held = true;
        }
        horiz_key_already_pressed = true;
    }
    else if (keystates[SDL_SCANCODE_LEFT]) {
        piece.horiz_direction = Piece::Direction::kLeft;
        if (horiz_key_already_pressed) {
            piece.horiz_key_held = true;
        }
        horiz_key_already_pressed = true;
    }
    else if (keystates[SDL_SCANCODE_RIGHT]) {
        piece.horiz_direction = Piece::Direction::kRight;
        if (horiz_key_already_pressed) {
            piece.horiz_key_held = true;
        }
        horiz_key_already_pressed = true;
    }
    else {
        piece.horiz_direction = Piece::Direction::kNull;
        horiz_key_already_pressed = false;
        piece.horiz_key_held = false;
    }
    if (keystates[SDL_SCANCODE_DOWN]) {
        piece.vert_direction = Piece::Direction::kDown;
        if (acceleration_key_already_pressed) {
            piece.acceleration_key_held = true;
        }
        acceleration_key_already_pressed = true;
    }
    else {
        piece.vert_direction = Piece::Direction::kNull;
        acceleration_key_already_pressed = false;
        piece.acceleration_key_held = false;
    }
    return false;
}
