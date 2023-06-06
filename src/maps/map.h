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
  void setHero(std::unique_ptr<IUnit> hero, const Coord& coord);

 protected:
  bool isExisted(const Coord& cd) const;
  std::vector<std::unique_ptr<Cell>> m_cellsInner;
  std::unordered_map<Coord, size_t, KeyHasher> m_cells;
  // std::map<int, size_t> m_cells;
};

#endif
