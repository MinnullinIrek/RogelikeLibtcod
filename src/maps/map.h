#ifndef MAP_H
#define MAP_H
#include <memory>
// #include <optional>
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
  const Coord& getSize() const;
  ~Map();

  Identifier getIdentifier(const Coord& cd);
  void setHero(std::shared_ptr<IUnit> hero, const Coord& coord);
  bool moveUnitFromTo(const Coord& currentPos, const Coord& nextPos);
  void setUnit(std::shared_ptr<IUnit> unit, const Coord& coord);
  std::shared_ptr<Cell> getCell(const Coord& cd) const;
  bool isWall(const Coord& cd) const;
 private:
  void setMover(std::shared_ptr<IUnit> unit, const Coord& coord);

 protected:
  bool isExisted(const Coord& cd) const;
  // std::vector<std::shared_ptr<Cell>> m_cellsInner;
  std::unordered_map<Coord, std::shared_ptr<Cell>, KeyHasher> m_cells;
  // std::map<int, size_t> m_cells;
  Coord m_size;
};

#endif
