#include "cell.h"

#include "../Bag.h"
#include "../header.h"
#include "../units/IUnit.h"

Cell::Cell() : m_id(emptyID), m_bag(std::make_shared<Bag>()) {}
Identifier Cell::toChar() const {
  if (m_isSeen) {
    return m_unit ? m_unit->toChar() : m_id;
  }
  return notSeen;
}
void Cell::setUnit(std::shared_ptr<IUnit> unit) { m_unit = unit; }
Cell::~Cell() {}
std::shared_ptr<IUnit> Cell::getUnit() { return m_unit; }
std::shared_ptr<Bag> Cell::getBag() { return m_bag; }
bool Cell::isWall() const { return m_unit && m_unit->m_type == EUnitTypes::wall; }
void Cell::setSeen(bool isSeen) { m_isSeen = isSeen; }
