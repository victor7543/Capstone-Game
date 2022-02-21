#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <fstream>
#include <iostream>
#include "SDL.h"
#include "json.hpp"

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
        key_pressed_timer(SDL_GetTicks()) 
  {
      std::ifstream stream("shapes.json");
      stream >> j;
      for (auto& a : j.items()) {
          auto b = a.value();
          for (auto& c : b.items()) {
              std::vector<std::string> s{ "t1", "t2", "t3", "t4" };
              for(auto x : s) {
                  if (c.key() == x) {
                      std::vector<float> e = c.value();
                      init_value.push_back(std::pair<float, float>(e[0], e[1]));
                      body.push_back(std::pair<float, float>(e[0] + head_x, e[1] + head_y));
                      std::cout << c.key() << "\t" << c.value() << std::endl;
                  }
              }
          }
          if (!body.empty())
              break;
      }
  }

  void Update();

  Direction direction = Direction::kDown;

  float speed_x{1.0f};
  float speed_y{0.0165};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::pair<float, float> prev_block_pos;
  std::vector<std::pair<float, float>> body;
  std::vector<std::pair<float, float>> init_value;

 private:
  bool growing{false};
  Direction prev_dir = Direction::kNeutral;
  int grid_width;
  int grid_height;
  Uint32 key_pressed_timer;
  Uint32 now;
  Uint32 time_diff;
  Uint32 time_threshold = 200;
  json j;
};

#endif