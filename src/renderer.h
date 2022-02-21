#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "shape.h"

using std::size_t;
using std::unique_ptr;

class Renderer {
 public:
  Renderer(const size_t screen_width, const size_t screen_height,
           const size_t grid_width, const size_t grid_height);
  ~Renderer();

  void Render(std::vector<unique_ptr<Shape>> &shape_vec, Shape const &shape);
 
  void UpdateWindowTitle(int score, int fps);

 private:
  void RenderShape(Shape const& shape, SDL_Rect& block);
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const size_t screen_width;
  const size_t screen_height;
  const size_t grid_width;
  const size_t grid_height;
};

#endif