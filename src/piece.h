#ifndef SHAPE_H
#define SHAPE_H

#include <random>
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

  enum class Direction { kUp, kDown, kLeft, kRight, kNull };
  enum class RotationDirection { Clockwise, Counter_Clockwise };

  void Move();
  bool TryRotate(vector<pair<float, float>>& prev_piece);
  void Rotate(vector<pair<float, float>> temp_initial_pos, RotationDirection rot_dir);

  Direction direction = Direction::kDown;

  const float speed_x = 1.0f;
  const float speed_y = 0.0165;
  bool is_falling = true;
  pair<float, float> prev_block_pos;
  vector<pair<float, float>> piece_pos;
  vector<pair<float, float>> base_pos;

 private:
  void CreatePiece(int grid_width);
  bool PreventOffScreenMovement(vector<pair<float, float>>& prev_piece, vector<pair<float, float>>& base_pos);
 
  Uint32 RandomizePiece(int pieces_count);
  Direction prev_dir = Direction::kNull;
  int grid_width;
  int grid_height;
  const std::string pieces_file = "pieces.json";
  Uint32 movement_key_timer = 0;
  Uint32 mov_time_threshold = 200;
  bool can_rotate = true;
  json json_content;
};

#endif