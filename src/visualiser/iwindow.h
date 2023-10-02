#ifndef IWINDOW_H
#define IWINDOW_H

#include <functional>

#include "../header.h"

struct IWindow {
  IWindow(const Rectangle& rectangle) : m_rectangle(rectangle) {}
  Rectangle m_rectangle;
  virtual void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) = 0;
};

#endif  // IWINDOW_H
