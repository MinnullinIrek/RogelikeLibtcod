#include "render.h"

#include <SDL.h>
#include <SDL_keycode.h>

#include <string>

#include "asset_manager.h"
#include "texture_manager.h"

#define width 500
#define height 500
#include <SDL.h>

// SDL_Renderer* Render::renderer;
SDL_Renderer* renderer;

Render::Render() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    throw "error initializing SDL";
  }
  window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
  if (!window) {
    throw "error creating window";
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    throw "error creating renderer";
  }
  std::string textureFilePath = "resources/assets/images/tank-big-right.png";
  // assetManager->addTexture("tank", textureFilePath.c_str());
  result = TextureManager::loadTexture(textureFilePath.c_str());
}

void Render::show() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);
  SDL_Rect projectile{10, 10, 10, 10};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &projectile);

  SDL_Rect sourceRectangle{0, 0, 1000, 1000};
  SDL_Rect destinationRectangle{0, 0, 100, 100};

  // TextureManager::draw(assetManager->getTexture("tank"), sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
  SDL_RenderCopyEx(renderer, result, &sourceRectangle, &destinationRectangle, 0.0, NULL, SDL_FLIP_NONE);
  SDL_RenderPresent(renderer);
}

Render::~Render() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
