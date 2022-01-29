#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
    now = steady_clock::now();
    time_diff = duration_cast<milliseconds>(now - key_pressed_timer).count();
    auto prev_body = body;
    if (prev_dir != direction || time_diff >= time_threshold) {
        float temp_speed_x = speed_x;
        if (time_diff < time_threshold) {
            key_pressed_timer = steady_clock::now();
        }
        else {
            temp_speed_x = speed_x / 5.0;
        }
        switch (direction) {
            case Direction::kUp:
                prev_dir = Direction::kUp;
                break;
            case Direction::kDown:
                body[0].second += speed_y * 10.0f;
                for (int i = 1; i < body.size(); i++) {
                    body[i].second = body[0].second + init_value[i].second;
                }
                prev_dir = Direction::kDown;
                break;
            case Direction::kLeft:
                body[0].first -= temp_speed_x;
                for (int i = 1; i < body.size(); i++) {
                    body[i].first = body[0].first + init_value[i].first;
                }
                prev_dir = Direction::kLeft;
                break;
            case Direction::kRight:
                body[0].first += temp_speed_x;
                for (int i = 1; i < body.size(); i++) {
                    body[i].first = body[0].first + init_value[i].first;
                }
                prev_dir = Direction::kRight;
                break;
            default:
                key_pressed_timer = steady_clock::now();
                prev_dir = Direction::kNeutral;
        }
    }
    direction = Direction::kNeutral;
    body[0].second += speed_y;
    for (int i = 1; i < body.size(); i++) {
        body[i].second = body[0].second + init_value[i].second;
    }
    // Wrap the Snake around to the beginning if going off of the screen.
    bool revert_position = false;
    bool revert_position_x = false;
    for (int i = 0; i < body.size(); i++) {
        if (body[i].first > grid_width) {
            revert_position_x = true;
        }
        if (body[i].first < 0.0) {
            revert_position_x = true;
        }
        if (body[i].second > grid_height) {
            revert_position = true;
            this->alive = false;
        }
        if (revert_position) {
            body[0].first = prev_body[0].first;
            body[0].second = prev_body[0].second;
            for (int i = 1; i < body.size(); i++) {
                body[i].first = body[0].first + init_value[i].first;
                body[i].second = body[0].second + init_value[i].second;
            }
            break;
        }
        else if (revert_position_x) {
            body[0].first = prev_body[0].first;
            for (int i = 1; i < body.size(); i++) {
                body[i].first = body[0].first + init_value[i].first;
            }
            break;
        }
    }
}
