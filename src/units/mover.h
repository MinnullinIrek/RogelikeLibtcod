#ifndef MOVER_H
#define MOVER_H

#include <memory>

#include "../header.h"

class Map;
class Cell;

class IMover {
 public:
  IMover(std::weak_ptr<Map> map);
  virtual void moveInDirection(const Coord& coord) = 0;
  void changeMap(std::weak_ptr<Map> map);
  virtual void setCoord(const Coord& currentPos) = 0;
  void innerMove(std::shared_ptr<Cell> cell1, std::shared_ptr<Cell> cell2);
  virtual const Coord& getCoord() const = 0;

 protected:
  std::weak_ptr<Map> m_map;
};

class SimpleMover : public IMover {
 public:
  SimpleMover(std::weak_ptr<Map> map);
  ~SimpleMover();

  virtual void moveInDirection(const Coord& coord) override;
  virtual const Coord& getCoord() const override;

  void setCoord(const Coord& currentPos) override;

 protected:
  Coord m_currentPos;
};

#endif
