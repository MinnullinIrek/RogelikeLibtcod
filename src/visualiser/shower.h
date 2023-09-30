#ifndef SHOWER_H
#define SHOWER_H

#include <libtcod.hpp>
#include <memory>
#include <functional>

#include "../header.h"

enum class EShower {
  map_shower,
  inventry_shower,

};

class Shower {
 public:
  virtual void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) = 0;
};

#endif
