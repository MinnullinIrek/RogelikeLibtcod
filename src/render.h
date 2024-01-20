#ifndef RENDER_H
#define RENDER_H

// #include <SDL2/SDL.h>
#include <SDL.h>
#include <SDL_render.h>

class AssetManager;
// class SDL_Renderer;

extern SDL_Renderer* renderer;

class Render {
 public:
  Render();
  ~Render();
  void render();
  // static SDL_Renderer* renderer;

 private:
  SDL_Window* window;
  AssetManager* assetManager;
  SDL_Texture* result;
};

#endif  //  RENDER_H
