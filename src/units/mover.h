#ifndef CELL_H
#define CELL_H
#include "../header.h"

class IMover {
 public:
  virtual void moveInDirection(const Coord& coord) = 0;
};

class SimpleMover : public IMover {
 public:
  virtual void moveInDirection(const Coord& coord) override;
};

#endif
