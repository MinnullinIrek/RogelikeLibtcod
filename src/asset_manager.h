#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <string_view>
#include <vector>

#include "texture_manager.h"

class AssetManager {
 public:
  // AssetManager();
  // ~AssetManager();
  // void clearData();
  void addTexture(const std::string& textureId, const char* filePath);
  SDL_Texture* getTexture(const std::string& textureId);

 private:
  std::vector<SDL_Texture*> textures;
  SDL_Texture* result;
};

#endif  // ASSET_MANAGER_H
