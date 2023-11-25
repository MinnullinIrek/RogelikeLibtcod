#ifndef MOVER_H
#define MOVER_H

#include <memory>

#include "../header.h"
#include "../utils/subscriber.h"
#include "mover_interface.h"

class Map;
class Cell;

class IMover : public MoverInterface {
 public:
  IMover(std::weak_ptr<Map> map);
  void changeMap(std::weak_ptr<Map> map);
  void innerMove(std::shared_ptr<Cell> cell1, std::shared_ptr<Cell> cell2);
  std::weak_ptr<Map> getMap();

 protected:
  std::weak_ptr<Map> m_map;
};

class SimpleMover : public IMover {
 public:
  SimpleMover(std::weak_ptr<Map> map);
  ~SimpleMover();

  virtual void moveInDirection(const Coord& coord) override;
  virtual const Coord& getCoord() const override;
  // virtual void move(std::shared_ptr<Unit> unit, std::shared_ptr<Cell> cell);
  void setCoord(const Coord& currentPos) override;

 protected:
  Coord m_currentPos;
};

#endif
