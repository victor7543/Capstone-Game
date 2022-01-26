#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>
#include "SDL.h"
#include "json.hpp"

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using json = nlohmann::json;

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight, kNeutral };
  enum Shape { O, I, S, Z, L, J, T};

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(0.0),
        key_pressed_timer(steady_clock::now()) 
  {
      std::ifstream stream("shapes.json");
      stream >> j;
      for (auto& a : j.items()) {
          auto b = a.value();
          for (auto& c : b.items()) {
              std::vector<std::string> s{ "t1", "t2", "t3", "t4" };
              for(auto x : s) {
                  if (c.key() == x) {
                      std::vector<int> e = c.value();
                      SDL_Point d{ e[0] + head_x, e[1] + head_y};
                      body.push_back(d);
                      std::cout << c.key() << "\t" << c.value() << std::endl;
                  }
              }
          }
          if (!body.empty())
              break;
      }
  }

  void Update();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kDown;

  float speed_x{1.0f};
  float speed_y{0.0165f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  float prev_head_x;
  float prev_head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  Direction prev_dir = Direction::kNeutral;
  int grid_width;
  int grid_height;
  steady_clock::time_point key_pressed_timer;
  steady_clock::time_point now;
  int time_diff;
  int time_threshold = 200;
  json j;
};

#endif