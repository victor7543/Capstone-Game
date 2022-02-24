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
          contolled_piece = make_unique<Piece>(grid_width, grid_height);
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
    controller.HandleInput(running, *contolled_piece);
    Update();
    renderer.Render(filled_cells , *contolled_piece);

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
    vector<SDL_Point> position;
    for (auto& cell : contolled_piece->piece_pos) {
        position.emplace_back(std::move(SDL_Point(
            static_cast<int>(cell.first),
            static_cast<int>(cell.second)
        )));
    }
    if (!contolled_piece->is_falling) {
        filled_cells.insert(std::end(filled_cells), std::begin(position), std::end(position));
        contolled_piece = make_unique<Piece>(_grid_width, _grid_height);
    }
    else {
        auto prev_pos = contolled_piece->piece_pos;
        vector<SDL_Point> prev_cell = position;
        contolled_piece->Move();
        DetectCollision(prev_pos, prev_cell);
    }
}

void Game::DetectCollision(vector<pair<float, float>> &prev_pos, vector<SDL_Point> &prev_cell)
{
    auto& piece = contolled_piece->piece_pos;
    for (int i = 0; i < piece.size(); i++) {
        contolled_piece->prev_block_pos = piece[i];
        SDL_Point current_cell{
            static_cast<int>(piece[i].first),
            static_cast<int>(piece[i].second)
        };
        for (auto const& cell : filled_cells) {
            if (current_cell.x != prev_cell[i].x || current_cell.y != prev_cell[i].y) { // The piece has changed cells since last update
                if (current_cell.x == cell.x && current_cell.y == cell.y) { // The piece is intersecting with a filled cell.
                    if (current_cell.x == prev_cell[i].x) {
                        piece[0] = prev_pos[0];
                        for (int j = 1; j < piece.size(); j++) {
                            piece[j].first = piece[0].first + contolled_piece->init_value[j].first;
                            piece[j].second = piece[0].second + contolled_piece->init_value[j].second;
                        }
                        contolled_piece->is_falling = false;
                        return;
                    }
                    else {
                        piece[0].first = prev_pos[0].first;
                        for (int j = 1; j < piece.size(); j++) {
                            piece[j].first = piece[0].first + contolled_piece->init_value[j].first;
                        }
                        return;
                    }
                }
            }
        }
    }
}

int Game::GetScore() const { return score; }