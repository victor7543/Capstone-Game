#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <SDL.h>
#include "json.hpp"

using json = nlohmann::json;
using std::vector;
using std::pair;

class Piece {
 public:
  Piece(int grid_width, int grid_height);

  enum class Direction { kUp, kDown, kLeft, kRight, kNull };
  enum class RotationDirection { Clockwise, Counter_Clockwise };

  void MoveHorizontal();
  void MoveVertical();
  void TryRotate();
  void Rotate(RotationDirection const &rot_dir);

  bool is_rotated = false;
  bool rotate_key_held = false;
  bool acceleration_key_held = false;
  bool horiz_key_held = false;
  Direction horiz_direction = Direction::kNull;
  Direction vert_direction = Direction::kNull;

  std::string color = "";
  const float speed_x = 1.0f;
  const float speed_y = 0.0165;
  bool is_falling = true;
  pair<float, float> prev_block_pos = {};
  vector<pair<float, float>> basic_shape = {};
  vector<pair<float, float>> current_pos = {};

 private:
  void CreatePiece(int grid_width);
  bool WindowBorderCollision(vector<pair<float, float>> const& prev_pos);
  bool WindowBorderCollision();
 
  Uint32 RandomizePiece(int pieces_count);
  Direction prev_dir = Direction::kNull;
  int grid_width = 0;
  int grid_height = 0;
  const std::string pieces_data_file_path = "assets/data/pieces.json";
  Uint32 horiz_mov_key_timer = 0;
  Uint32 vert_mov_key_timer = 0;
  Uint32 vert_mov_time_threshold = 50;
  Uint32 horiz_mov_time_threshold = 150;
  bool can_rotate = true;
  json json_file;
};

#endif