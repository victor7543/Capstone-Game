#ifndef GAME_H
#define GAME_H
#ifndef SDL_MAIN_HANDLED
    #define SDL_MAIN_HANDLED
    #include <SDL.h>
#endif

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;
using std::pair;

class Controller;
class Renderer;
class Piece;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);

 private:
  vector<SDL_Point> filled_cells;
  vector<SDL_Point> piece_cells;
  vector<SDL_Point> prev_piece_cells;
  vector<pair<float, float>> prev_pos;
  SDL_Point current_cell = {};

  bool game_over = false;
  int score = 0;
  size_t _grid_width = 0;
  size_t _grid_height = 0;
  unique_ptr<Piece> controlled_piece = nullptr;
  Uint32 wavLength = 0;
  Uint8* wavBuffer = nullptr;
  SDL_AudioDeviceID deviceId = UINT32_MAX;
  const char* audio_file_path = "assets/audio/Tetris_theme.wav";

  bool InitAudio(SDL_AudioSpec& wavSpec);
  void Update();
  void CheckCompletedRows();
  bool DetectCollisions();
  void ValidateVerticalMovement();
  void ValidateHorizontalMovement();
  void ValidateRotation();
  bool CheckGameOver();
  void ResetGame();
};

#endif