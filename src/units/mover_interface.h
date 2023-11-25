#ifndef MOVER_INTERFACE_H
#define MOVER_INTERFACE_H

#include <memory>

#include "../header.h"
#include "../utils/subscriber.h"

class Map;

class MoverInterface : public Publisher {
 public:
  virtual void moveInDirection(const Coord& coord) = 0;
  virtual void setCoord(const Coord& currentPos) = 0;
  virtual const Coord& getCoord() const = 0;
  virtual std::weak_ptr<Map> getMap() = 0;
  virtual void changeMap(std::weak_ptr<Map> map) = 0;
};

#endif  // MOVER_INTERFACE_H
