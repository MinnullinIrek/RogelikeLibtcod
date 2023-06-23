#ifndef MAP_H
#define MAP_H
#include <memory>
#include <unordered_map>
#include <vector>

#include "../header.h"

class Cell;
class IUnit;

class Map {
 private:
 public:
  Map();
  Map(const Coord& size);
  void setSize(const Coord& size);
  ~Map();

  Identifier getIdentifier(const Coord& cd);
  void setHero(std::shared_ptr<IUnit> hero, const Coord& coord);
  bool moveUnitFromTo(const Coord& currentPos, const Coord& nextPos);
  
 private:
  void setMover(std::shared_ptr<IUnit> unit, const Coord& coord);
  void swapUnit(std::shared_ptr<Cell> cellStart, std::shared_ptr<Cell> endStart);
 protected:
  bool isExisted(const Coord& cd) const;
  std::vector<std::shared_ptr<Cell>> m_cellsInner;
  std::unordered_map<Coord, size_t, KeyHasher> m_cells;
  // std::map<int, size_t> m_cells;
};

#endif
