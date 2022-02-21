#include "shape.h"
#include <cmath>
#include <iostream>

void Shape::Update() {
    now = SDL_GetTicks();
    time_diff = (now - key_pressed_timer);
    auto prev_shape = shape_cells;
    if (prev_dir != direction || time_diff >= time_threshold) {
        float temp_speed_x = speed_x;
        if (time_diff < time_threshold) {
            key_pressed_timer =SDL_GetTicks();
        }
        else {
            temp_speed_x = speed_x / 5.0;
        }
        switch (direction) {
            case Direction::kUp:
                prev_dir = Direction::kUp;
                break;
            case Direction::kDown:
                shape_cells[0].second += speed_y * 10.0f;
                for (int i = 1; i < shape_cells.size(); i++) {
                    shape_cells[i].second = shape_cells[0].second + init_value[i].second;
                }
                prev_dir = Direction::kDown;
                break;
            case Direction::kLeft:
                shape_cells[0].first -= temp_speed_x;
                for (int i = 1; i < shape_cells.size(); i++) {
                    shape_cells[i].first = shape_cells[0].first + init_value[i].first;
                }
                prev_dir = Direction::kLeft;
                break;
            case Direction::kRight:
                shape_cells[0].first += temp_speed_x;
                for (int i = 1; i < shape_cells.size(); i++) {
                    shape_cells[i].first = shape_cells[0].first + init_value[i].first;
                }
                prev_dir = Direction::kRight;
                break;
            default:
                key_pressed_timer = SDL_GetTicks();
                prev_dir = Direction::kNeutral;
        }
    }
    direction = Direction::kNeutral;
    shape_cells[0].second += speed_y;
    for (int i = 1; i < shape_cells.size(); i++) {
        shape_cells[i].second = shape_cells[0].second + init_value[i].second;
    }
    // Revert the shape's position if going off of the screen.
    bool revert_position = false;
    bool revert_position_x = false;
    for (int i = 0; i < shape_cells.size(); i++) {
        if (shape_cells[i].first > grid_width) {
            revert_position_x = true;
        }
        if (shape_cells[i].first < 0.0) {
            revert_position_x = true;
        }
        if (shape_cells[i].second > grid_height) {
            revert_position = true;
            this->is_falling = false;
        }
        if (revert_position) {
            shape_cells[0].first = prev_shape[0].first;
            shape_cells[0].second = prev_shape[0].second;
            for (int i = 1; i < shape_cells.size(); i++) {
                shape_cells[i].first = shape_cells[0].first + init_value[i].first;
                shape_cells[i].second = shape_cells[0].second + init_value[i].second;
            }
            break;
        }
        else if (revert_position_x) {
            shape_cells[0].first = prev_shape[0].first;
            for (int i = 1; i < shape_cells.size(); i++) {
                shape_cells[i].first = shape_cells[0].first + init_value[i].first;
            }
            break;
        }
    }
}
