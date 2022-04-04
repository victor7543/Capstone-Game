#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char* args[]) {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{480};
  constexpr std::size_t kScreenHeight{960};
  constexpr std::size_t kGridWidth{10};
  constexpr std::size_t kGridHeight{20};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  return 0;
}