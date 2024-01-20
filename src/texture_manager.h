#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL.h>

class TextureManager {
 private:
  /* data */
 public:
  // TextureManager(/* args */);
  // ~TextureManager();

  static SDL_Texture* loadTexture(const char* fileName);
  // static void draw(
  // SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip);
};

#endif  //  TEXTURE_MANAGER_H
