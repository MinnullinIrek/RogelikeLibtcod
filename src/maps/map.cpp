#include "map.h"

#include <assert.h>

#include "../units/IUnit.h"
#include "cell.h"

Map::Map(){};

Map::Map(const Coord& size) { setSize(size); }

Map::~Map() {}

void Map::setSize(const Coord& size) {
  m_cells.clear();

  for (decltype(size.x) x = 0; x < size.x; ++x) {
    for (decltype(size.y) y = 0; y < size.y; ++y) {
      m_cells.insert({Coord{x, y}, std::make_shared<Cell>()});
    }
  }
}

Identifier Map::getIdentifier(const Coord& cd) {
  if (isExisted(cd)) {
    return m_cells[cd]->toChar();
  }

  return emptyID;
}

bool Map::isExisted(const Coord& cd) const { return m_cells.find(cd) != m_cells.end(); }

void Map::setHero(std::shared_ptr<IUnit> hero, const Coord& coord) {
  assert(isExisted(coord));
  m_cells.at(coord)->setUnit(hero);
  setMover(hero, coord);
}

bool Map::moveUnitFromTo(const Coord& currentPos, const Coord& nextPos) {
  assert(isExisted(currentPos));

  bool result;

  if (isExisted(nextPos)) {
    auto cell1 = m_cells.at(currentPos);
    auto cell2 = m_cells.at(nextPos);
    auto unit1 = cell1->getUnit();
    auto unit2 = cell2->getUnit();

    if (unit2) {
    } else {
      cell2->setUnit(unit1);
      cell1->setUnit(nullptr);
    }
    //  cells.emplace(std::make_pair(cell1, cell2));
  }

  return result;
}

void Map::setUnit(std::shared_ptr<IUnit> unit, const Coord& coord) {
  assert(isExisted(coord));
  auto cell = m_cells.at(coord);
  assert(!cell->getUnit());
  m_cells.at(coord)->setUnit(unit);
}

std::shared_ptr<Cell> Map::getCell(const Coord& cd) const {
  if (isExisted(cd)) {
    m_cells.at(cd);
  }
  return nullptr;
}
