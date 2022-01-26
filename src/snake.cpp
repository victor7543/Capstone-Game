#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
    now = steady_clock::now();
    time_diff = duration_cast<milliseconds>(now - key_pressed_timer).count();
    if (prev_dir != direction || time_diff > time_threshold) {
        float temp_speed_x = speed_x;
        if (time_diff < time_threshold) {
            key_pressed_timer = steady_clock::now();
        }
        else {
            temp_speed_x = speed_x / 5.0;
        }
        switch (direction) {
        case Direction::kUp:
            //head_y -= speed;
            prev_dir = Direction::kUp;
            break;

        case Direction::kDown:
            head_y += speed_y * 10.0f;
            prev_dir = Direction::kDown;
            break;

        case Direction::kLeft:
            head_x -= temp_speed_x;
            prev_dir = Direction::kLeft;
            break;

        case Direction::kRight:

            head_x += temp_speed_x;
            prev_dir = Direction::kRight;
            break;
        default:
            key_pressed_timer = steady_clock::now();
            prev_dir = Direction::kNeutral;
        }
    }
    direction = Direction::kNeutral;
    head_y += speed_y;
    // Wrap the Snake around to the beginning if going off of the screen.
    if (head_x > (grid_width - 1.0)) {
        head_x = grid_width - 1.0;
    }
    if (head_x < 0.0) {
        head_x = 0.0;
    }
    if (head_y > (grid_height - 1.0)) {
        head_y = grid_height - 1.0;
        this->alive = false;
    }
  //head_x = fmod(head_x + grid_width, grid_width);
  //head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector

  // Check if the snake has died.
}


// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}