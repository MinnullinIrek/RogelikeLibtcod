#ifndef CELL_H
#define CELL_H

#include <memory>
#include "../header.h"

class Map;

class IMover {
 public:
  IMover(std::weak_ptr<Map> map);
  virtual void moveInDirection(const Coord& coord) = 0;
  void changeMap(std::weak_ptr<Map> map);
  virtual void setCoord(const Coord& currentPos) = 0;
 protected:
  std::weak_ptr<Map> m_map;
};

class SimpleMover : public IMover {
 public:
  SimpleMover(std::weak_ptr<Map> map);
  ~SimpleMover();

  virtual void moveInDirection(const Coord& coord) override;
  void setCoord(const Coord& currentPos) override;

  protected:
  Coord m_currentPos;

};

#endif
