#ifndef IWINDOW_H
#define IWINDOW_H

#include <functional>

#include "../header.h"

class IWindow {
 public:
  IWindow(const Rectangle& rectangle) : m_rectangle(rectangle) {}
  virtual void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) const = 0;
  Rectangle m_rectangle;

 protected:
};

#endif  // IWINDOW_H
