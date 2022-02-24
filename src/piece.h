#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <fstream>
#include <iostream>
#include "SDL.h"
#include "json.hpp"

using json = nlohmann::json;
using std::vector;
using std::pair;

class Piece {
 public:
  Piece(int grid_width, int grid_height);

  enum class Direction { kUp, kDown, kLeft, kRight, kNeutral };
  enum PieceType { O, I, S, Z, L, J, T };

  void Move();

  Direction direction = Direction::kDown;

  float speed_x{1.0f};
  float speed_y{0.0165};
  int size{1};
  bool is_falling{true};
  pair<float, float> prev_block_pos;
  vector<pair<float, float>> piece_pos;
  vector<pair<float, float>> init_value;

 private:
  void CreatePiece(int grid_width);
  void PreventOffScreenMovement(vector<pair<float, float>> &prev_piece);
  Direction prev_dir = Direction::kNeutral;
  int grid_width;
  int grid_height;
  const std::string pieces_file = "pieces.json";
  Uint32 key_pressed_timer = 0;
  Uint32 now = 0;
  Uint32 time_diff = 0;
  Uint32 time_threshold = 200;
  json json_content;
};

#endif