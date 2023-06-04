#include "cell.h"

#include "../Bag.h"
#include "../header.h"
#include "../units/IUnit.h"

Cell::Cell() : m_id(emptyID) {}
Identifier Cell::toChar() const { return m_id; }
// void Cell::setUnit(std::unique_ptr<IUnit> unit) { m_unit = std::move(unit); }
Cell::~Cell() {}
