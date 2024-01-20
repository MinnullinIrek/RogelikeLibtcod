#include "asset_manager.h"

// AssetManager::AssetManager() {}
// AssetManager::~AssetManager() {}
// void AssetManager::clearData() { textures.clear(); }
void AssetManager::addTexture(const std::string& textureId, const char* filePath) {
  auto result = TextureManager::loadTexture(filePath);
  // textures.push_back(result);
}

SDL_Texture* AssetManager::getTexture(const std::string& textureId) {
  return result;
  // return textures.front();
}
