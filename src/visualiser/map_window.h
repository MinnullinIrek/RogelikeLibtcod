#ifndef MAP_WINDOW_H
#define MAP_WINDOW_H

#include <unordered_map>

#include "../header.h"
#include "iwindow.h"

class MapWindow : public IWindow {
 public:
  MapWindow(const Rectangle& r);
  virtual void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) override;

 private:
  Rectangle m_rectangle;
  std::unordered_map<Coord, Text, KeyHasher> m_cells;
};

#endif  // MAP_WINDOW_H
