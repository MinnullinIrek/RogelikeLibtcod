#include "map.h"

#include <assert.h>

#include "../units/IUnit.h"
#include "../units/interactor.h"
#include "../units/mover.h"
#include "cell.h"

Map::Map(){};

Map::Map(const Coord& size) : m_size(size) { setSize(size); }

Map::~Map() {}

const Coord& Map::getSize() const { return m_size; }

void Map::setSize(const Coord& size) {
  m_size = size;
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

  bool result = false;

  if (isExisted(nextPos)) {
    auto& cell1 = m_cells.at(currentPos);
    auto& cell2 = m_cells.at(nextPos);
    auto unit1 = cell1->getUnit();
    auto unit2 = cell2->getUnit();
    assert(unit1);
    if (unit2) {
      auto interactor = unit1->getInteractor();
      assert(interactor);
      interactor->interact(unit1, unit2);

    } else {
      auto unit = std::dynamic_pointer_cast<Unit>(unit1);
      /*if (unit) {
        auto mover = unit->getMover();
        mover->move(unit, cell2);
      }*/
      cell2->setUnit(unit1);
      cell1->setUnit(nullptr);
      result = true;
    }
  }
  if (result) {
    emit();
  }
  return result;
}

void Map::setUnit(std::shared_ptr<IUnit> unit, const Coord& coord) {
  // assert(isExisted(coord));
  if (!isExisted(coord)) {
    throw std::string("can't set unit to map") + unit->toChar().symbol + "to coord = " + coord.toString();
  }

  // eror assert
  m_cells.at(coord)->setUnit(unit);
  if (unit) {
    unit->setCoord(coord);
  }
}

std::shared_ptr<Cell> Map::getCell(const Coord& cd) const {
  if (isExisted(cd)) {
    return m_cells.at(cd);
  }
  return nullptr;
}

void Map::setSeen(const Coord& cd) {
  if (isExisted(cd)) {
    m_cells.at(cd)->setSeen(true);
  }
}

bool Map::isWall(const Coord& cd) const {
  auto cell = getCell(cd);
  return cell && cell->isWall();
}
