#include <iostream>
#include <string>
#include "renderer.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL_ttf.h>
#elif __APPLE__ || __linux__
#include <SDL2/SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
#endif

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

void Renderer::Render(std::vector<SDL_Point>& cells_vec, Piece const &piece, int score, bool game_over) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
  for (unsigned int i = 0; i < cells_vec.size(); i++) {
      RenderFilledCells(cells_vec[i], block);
  } 
  if (!game_over) {
      for (auto& cell : piece.current_pos) {
          SDL_Point cell_point = SDL_Point();
          cell_point.x = static_cast<int>(cell.first);
          cell_point.y = static_cast<int>(cell.second);
          RenderPiece(cell_point, block, piece);
      }
  }
  RenderScore(score);
  if (game_over) {
      RenderGameOver();
  }
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderPiece(SDL_Point const& point, SDL_Rect& block, Piece const& piece)
{
    // Render piece
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0x00);
    block.x = static_cast<int>(point.x) * block.w;
    block.y = static_cast<int>(point.y) * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
    // scale down the rect and render again to create an outline
    SDL_Rect interior_color = block;
    interior_color.h *= 0.95;
    interior_color.w *= 0.95;
    std::string red = piece.color.substr(1, 2);
    std::string green = piece.color.substr(3, 2);
    std::string blue = piece.color.substr(5, 2);
    SDL_SetRenderDrawColor(sdl_renderer, std::stoul(red, nullptr, 16), std::stoul(green, nullptr, 16), std::stoul(blue, nullptr, 16), 0xFF);
    SDL_RenderFillRect(sdl_renderer, &interior_color);
}

void Renderer::RenderFilledCells(SDL_Point const& point, SDL_Rect& block)
{
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0x00);
    block.x = static_cast<int>(point.x) * block.w;
    block.y = static_cast<int>(point.y) * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);

    // scale down the rect and render again to create an outline
    SDL_Rect interior_color = block;
    interior_color.h *= 0.95;
    interior_color.w *= 0.95;
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(sdl_renderer, &interior_color);
}

void Renderer::RenderScore(int score)
{
    std::string score_text = "Score: " + std::to_string(score);
    TTF_Font* font = TTF_OpenFont(font_path, 24);
    SDL_Color White = { 255, 255, 255, 255 };
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid(font, score_text.c_str(), White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(sdl_renderer, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = 0;
    Message_rect.y = 0;
    Message_rect.w = 100;
    Message_rect.h = 30;
    SDL_RenderCopy(sdl_renderer, Message, NULL, &Message_rect);
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(font);
}

void Renderer::RenderGameOver()
{
    std::string game_over_text = "Press any key to play again";
    TTF_Font* font = TTF_OpenFont(font_path, 36);
    if (!font) {
        std::cout << "Error opening font file from path: '" << font_path << "'" << std::endl;
    }
    SDL_Color Orange = { 255, 99, 71, 255 };
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid(font, game_over_text.c_str(), Orange);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(sdl_renderer, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.w = screen_width - screen_width / 6;
    Message_rect.h = screen_width / 10;
    Message_rect.x = screen_width / 12;
    Message_rect.y = screen_height / 2 - Message_rect.h;
    SDL_RenderCopy(sdl_renderer, Message, NULL, &Message_rect);
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(font);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
