#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <chrono>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

using std::vector;
using std::unique_ptr;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  unique_ptr<Snake> snake;
  vector<unique_ptr<Snake>> snake_vec;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  size_t _grid_width;
  size_t _grid_height;

  void PlaceFood();
  void Update();
};

#endif