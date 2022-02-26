#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "piece.h"

using std::vector;
using std::unique_ptr;
using std::pair;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;

 private:
  unique_ptr<Piece> controlled_piece;
  vector<SDL_Point> filled_cells;

  int score{0};
  size_t _grid_width;
  size_t _grid_height;

  void Update();
  void DetectCollision(vector<pair<float, float>> &prev_piece, vector<SDL_Point> &prev_cell, bool rotated);
};

#endif