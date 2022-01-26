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
  snake = make_unique<Snake>(grid_width, grid_height);
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
    controller.HandleInput(running, *snake);
    Update();
    renderer.Render(snake_vec , *snake, food);

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

void Game::PlaceFood() {
  int x, y;
  bool first_time = true;
  while (true) {
      if (first_time) {
          x = 16;
          y = 31;
          first_time = false;
      }
      else {
          x = random_w(engine);
          y = random_h(engine);
      }
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
    if (!snake->alive) {
        snake_vec.emplace_back(std::move(snake));
        snake = make_unique<Snake>(_grid_width, _grid_height);
    }
    else {
        SDL_Point prev_cell{
            static_cast<int>(snake->head_x),
            static_cast<int>(snake->head_y) 
        };
        snake->prev_head_x = snake->head_x;
        snake->prev_head_y = snake->head_y;
        snake->Update();
        SDL_Point current_cell{
            static_cast<int>(snake->head_x),
            static_cast<int>(snake->head_y)
        };
        for (auto const& item : snake_vec) {
            if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
                SDL_Point item_cell{
                    static_cast<int>(item->head_x),
                    static_cast<int>(item->head_y)
                };
                if (current_cell.x == item_cell.x && current_cell.y == item_cell.y) {
                    snake->head_x = snake->prev_head_x;
                    snake->head_y = snake->prev_head_y;
                    snake->alive = false;
                }
            }
        }
    }


  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);

}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake->size; }