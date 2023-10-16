#include "mover.h"

#include "../maps/cell.h"
#include "../maps/map.h"
#include "IUnit.h"

IMover::IMover(std::weak_ptr<Map> map) : m_map(map) {}

// cIMover() {}
// void IMover::setToCell(const Coord& currentPos) {
// m_currentPos = currentPos;
//};

void IMover::changeMap(std::weak_ptr<Map> map) { m_map = map; }

// void IMover::moveInDirection(const Coord&) {}

void SimpleMover::moveInDirection(const Coord& coord) {
  auto map = m_map.lock();
  if (map) {
    auto newCoord = m_currentPos + coord;

    auto cell1 = map->getCell(m_currentPos);
    auto cell2 = map->getCell(m_currentPos + coord);

    auto result = map->moveUnitFromTo(m_currentPos, newCoord);
    if (result) {
      m_currentPos = newCoord;
    }
    emit();
    // if (result.has_value()) {
    //   auto cells = result.value();
    //   std::shared_ptr<Cell> cell1 = cells.first;
    //   auto cell2 = cells.second;
    //   // auto unit1 = cell1->getUnit();
    //   // cell1
    //   std::shared_ptr<Cell> c;

    //   m_currentPos = newCoord;
    // }
  }
}

void SimpleMover::setCoord(const Coord& currentPos) { m_currentPos = currentPos; }

// IMover::~IMover() {}

SimpleMover::SimpleMover(std::weak_ptr<Map> map) : IMover(map) {}

SimpleMover::~SimpleMover() {}

const Coord& SimpleMover::getCoord() const { return m_currentPos; }

std::weak_ptr<Map> IMover::getMap() { return m_map; }
