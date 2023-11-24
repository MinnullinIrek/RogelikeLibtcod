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
    auto result = map->moveUnitFromTo(m_currentPos, newCoord);
    if (result) {
      m_currentPos = newCoord;
    }
  }
}

void SimpleMover::setCoord(const Coord& currentPos) { m_currentPos = currentPos; }
//void SimpleMover::move(std::shared_ptr<Unit> unit, std::shared_ptr<Cell> cell) {
  //if (unit->upTime(distance)) {
  //}
//}

// IMover::~IMover() {}

SimpleMover::SimpleMover(std::weak_ptr<Map> map) : IMover(map) {}

SimpleMover::~SimpleMover() {}

const Coord& SimpleMover::getCoord() const { return m_currentPos; }

std::weak_ptr<Map> IMover::getMap() { return m_map; }
