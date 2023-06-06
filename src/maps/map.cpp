#include "map.h"

#include "../units/IUnit.h"
#include "cell.h"

Map::Map(){};

Map::Map(const Coord& size) { setSize(size); }

Map::~Map() {}

void Map::setSize(const Coord& size) {
  m_cellsInner.clear();
  m_cells.clear();
  m_cellsInner.reserve(size.x * size.y);

  for (decltype(size.x) x = 0; x < size.x; ++x) {
    for (decltype(size.y) y = 0; y < size.y; ++y) {
      m_cellsInner.push_back(std::make_unique<Cell>());
      m_cells.insert({Coord{x, y}, m_cellsInner.size() - 1});
    }
  }
}

Identifier Map::getIdentifier(const Coord& cd) {
  if (isExisted(cd)) {
    return m_cellsInner.at(m_cells[cd])->toChar();
  }

  return emptyID;
}

bool Map::isExisted(const Coord& cd) const { return m_cells.find(cd) != m_cells.end(); }

void Map::setHero(std::unique_ptr<IUnit> hero, const Coord& coord) {
  if (isExisted(coord)) {
    m_cellsInner.at(m_cells.at(coord))->setUnit(std::move(hero));
  }
}