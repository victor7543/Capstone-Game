#ifndef GAME_H
#define GAME_H

#include <vector>
#include "SDL.h"

using std::vector;
using std::unique_ptr;
using std::pair;

class Controller;
class Renderer;
class Piece;
class SDL_Point;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);

 private:
  unique_ptr<Piece> controlled_piece;
  vector<SDL_Point> filled_cells;
  vector<SDL_Point> piece_cells;
  vector<SDL_Point> prev_piece_cells;
  vector<pair<float, float>> prev_pos;

  bool is_game_over = false;
  int score = 0;
  size_t _grid_width;
  size_t _grid_height;

  Uint32 wavLength = 0;
  Uint8* wavBuffer = nullptr;
  SDL_AudioDeviceID deviceId = 0;

  bool InitAudio(SDL_AudioSpec& wavSpec);
  void Update();
  void VerifyCompletedRows();
  void DetectCollisions(bool rotated);
  void ResetGame();
};

#endif