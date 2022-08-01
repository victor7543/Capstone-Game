#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{400};
  constexpr std::size_t kScreenHeight{800};
  constexpr std::size_t kGridWidth{10};
  constexpr std::size_t kGridHeight{20};
  try {
	  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
	  Controller controller;
	  Game game(kGridWidth, kGridHeight);
	  game.Run(controller, renderer, kMsPerFrame);
	  std::cout << "Game has terminated successfully!\n";
  }
  catch (...) {
	  std::cout << "An unexpected error has caused the game to terminate immediately." << std::endl;
  }
  return 0;
}