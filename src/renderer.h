#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "piece.h"

using std::size_t;

class Renderer {
 public:
  Renderer(const size_t screen_width, const size_t screen_height,
           const size_t grid_width, const size_t grid_height);
  ~Renderer();

  void Render(std::vector<SDL_Point> &piece_vec, Piece const &piece);
 
  void UpdateWindowTitle(int score, int fps);

 private:
  void RenderPiece(SDL_Point const& point, SDL_Rect& block);
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const size_t screen_width;
  const size_t screen_height;
  const size_t grid_width;
  const size_t grid_height;
};

#endif