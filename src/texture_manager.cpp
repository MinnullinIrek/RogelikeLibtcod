#include "texture_manager.h"

#include <SDL_image.h>

#include "render.h"

SDL_Texture* TextureManager::loadTexture(const char* fileName) {
  SDL_Surface* surface = IMG_Load(fileName);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

// void draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip) {
// SDL_RenderCopyEx(renderer, texture, &sourceRectangle, &destinationRectangle, 0.0, NULL, flip);
// }
