#include "IUnit.h"

#include "../Bag.h"
#include "../header.h"

IUnit::IUnit() {}
IUnit::IUnit(Identifier id) : m_id(id) {}

Identifier IUnit::toChar() const { return m_id; }

Unit::Unit(const Identifier& id) : IUnit(id) {}
Unit::Unit() : IUnit() {}

// Identifier Unit::toChar() const { return m_id; }

Description Unit::toString() { return Description("decription"); }
