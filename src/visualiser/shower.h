#ifndef SHOWER_H
#define SHOWER_H

#include <libtcod.hpp>
#include <memory>

enum class EShower {
  map_shower,
  inventry_shower,

};

class Shower {
 public:
  virtual void show(tcod::Console& console) = 0;
};

#endif
