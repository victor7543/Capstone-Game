#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(std::vector<SDL_Point>& cells_vec, Piece const &piece, int score) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  for (int i = 0; i < cells_vec.size(); i++) {
      RenderPiece(cells_vec[i], block);
  } 
  for (auto& cell : piece.piece_pos) {
      RenderPiece(SDL_Point(static_cast<int>(cell.first), static_cast<int>(cell.second)), block);
  }
  RenderScore(score);
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderPiece(SDL_Point const& point, SDL_Rect& block)
{
    // Render piece
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    block.x = static_cast<int>(point.x) * block.w;
    block.y = static_cast<int>(point.y) * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::RenderScore(int score)
{
    std::string score_text = "Score: " + std::to_string(score);
    TTF_Font* Arial = TTF_OpenFont("Arial.ttf", 24);
    SDL_Color White = { 255, 255, 255 };
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid(Arial, score_text.c_str(), White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(sdl_renderer, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = 0;
    Message_rect.y = 0;
    Message_rect.w = 100;
    Message_rect.h = 30;
    SDL_RenderCopy(sdl_renderer, Message, NULL, &Message_rect);
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
