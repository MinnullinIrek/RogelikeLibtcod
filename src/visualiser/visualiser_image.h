#ifndef VISUALISER_IMAGE_H
#define VISUALISER_IMAGE_H

#include <SDL.h>
#include <SDL_render.h>

#include "../header.h"
#include "visualiser_interface.h"

class VisualiserImage : public VisualiserInterface {
 private:
  /* data */
 public:
  VisualiserImage(const Coord& windowSize);
  ~VisualiserImage();
  void show() const override;
  void addWindow(std::shared_ptr<IWindow> window) override;

  // SDL_Renderer* renderer;
  // SDL_Window* window;
  // AssetManager* assetManager;
  // SDL_Texture* result;
};

#endif  // VISUALISER_IMAGE_H
