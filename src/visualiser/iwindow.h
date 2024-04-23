#ifndef IWINDOW_H
#define IWINDOW_H

#include <functional>

#include "../header.h"

class IWindow {
 public:
  IWindow(const RectangleS& rectangle) : m_rectangle(rectangle) {}
  virtual void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) = 0;
  RectangleS m_rectangle;

 protected:
};

#endif  // IWINDOW_H
