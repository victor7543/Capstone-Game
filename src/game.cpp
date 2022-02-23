#include "game.h"
#include <iostream>
#include "SDL.h"

using std::make_unique;
using std::size_t;

Game::Game(size_t grid_width, size_t grid_height)
    : engine(dev()),
      _grid_height(grid_height),
      _grid_width(grid_width),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
          shape_ptr = make_unique<Shape>(grid_width, grid_height);
}

void Game::Run(Controller const &controller, Renderer &renderer,
               size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, *shape_ptr);
    Update();
    renderer.Render(filled_cells , *shape_ptr);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::Update() {
    auto& shape = shape_ptr->shape_pos;
    vector<SDL_Point> position;
    for (auto& cell : shape) {
        position.emplace_back(std::move(SDL_Point(
            static_cast<int>(cell.first),
            static_cast<int>(cell.second)
        )));
    }
    if (!shape_ptr->is_falling) {
        filled_cells.insert(std::end(filled_cells), std::begin(position), std::end(position));
        shape_ptr = make_unique<Shape>(_grid_width, _grid_height);
    }
    else {
        auto prev_shape = shape;
        vector<SDL_Point> prev_position = position;
        shape_ptr->Update();
        for (int i = 0; i < shape.size(); i++) {
            shape_ptr->prev_block_pos = shape[i];
            SDL_Point current_cell{
                static_cast<int>(shape[i].first),
                static_cast<int>(shape[i].second)
            };
            for (auto const& cell : filled_cells) {
                if (current_cell.x != prev_position[i].x || current_cell.y != prev_position[i].y) {
                    if (current_cell.x == cell.x && current_cell.y == cell.y) {
                        if ((prev_position[i].x == current_cell.x)) {
                            shape[0] = prev_shape[0];
                            for (int j = 1; j < shape.size(); j++) {
                                shape[j].first = shape[0].first + shape_ptr->init_value[j].first;
                                shape[j].second = shape[0].second + shape_ptr->init_value[j].second;
                            }
                            shape_ptr->is_falling = false;
                            return;
                        }
                        else {
                            shape[0].first = prev_shape[0].first;
                            for (int j = 1; j < shape.size(); j++) {
                                shape[j].first = shape[0].first + shape_ptr->init_value[j].first;
                            }
                            return;
                        }
                    }
                }
            }
        }   
    }
}

int Game::GetScore() const { return score; }