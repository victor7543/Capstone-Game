#include "shape.h"
#include <cmath>
#include <iostream>

void Shape::Update() {
    now = SDL_GetTicks();
    time_diff = (now - key_pressed_timer);
    auto prev_shape = shape_pos;
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
                shape_pos[0].second += speed_y * 10.0f;
                for (int i = 1; i < shape_pos.size(); i++) {
                    shape_pos[i].second = shape_pos[0].second + init_value[i].second;
                }
                prev_dir = Direction::kDown;
                break;
            case Direction::kLeft:
                shape_pos[0].first -= temp_speed_x;
                for (int i = 1; i < shape_pos.size(); i++) {
                    shape_pos[i].first = shape_pos[0].first + init_value[i].first;
                }
                prev_dir = Direction::kLeft;
                break;
            case Direction::kRight:
                shape_pos[0].first += temp_speed_x;
                for (int i = 1; i < shape_pos.size(); i++) {
                    shape_pos[i].first = shape_pos[0].first + init_value[i].first;
                }
                prev_dir = Direction::kRight;
                break;
            default:
                key_pressed_timer = SDL_GetTicks();
                prev_dir = Direction::kNeutral;
        }
    }
    direction = Direction::kNeutral;
    shape_pos[0].second += speed_y;
    for (int i = 1; i < shape_pos.size(); i++) {
        shape_pos[i].second = shape_pos[0].second + init_value[i].second;
    }
    // Revert the shape's position if going off of the screen.
    bool revert_position = false;
    bool revert_position_x = false;
    for (int i = 0; i < shape_pos.size(); i++) {
        if (shape_pos[i].first > grid_width) {
            revert_position_x = true;
        }
        if (shape_pos[i].first < 0.0) {
            revert_position_x = true;
        }
        if (shape_pos[i].second > grid_height) {
            revert_position = true;
            this->is_falling = false;
        }
        if (revert_position) {
            shape_pos[0].first = prev_shape[0].first;
            shape_pos[0].second = prev_shape[0].second;
            for (int i = 1; i < shape_pos.size(); i++) {
                shape_pos[i].first = shape_pos[0].first + init_value[i].first;
                shape_pos[i].second = shape_pos[0].second + init_value[i].second;
            }
            break;
        }
        else if (revert_position_x) {
            shape_pos[0].first = prev_shape[0].first;
            for (int i = 1; i < shape_pos.size(); i++) {
                shape_pos[i].first = shape_pos[0].first + init_value[i].first;
            }
            break;
        }
    }
}
