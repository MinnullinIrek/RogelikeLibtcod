#include "IUnit.h"

#include "../Bag.h"
#include "../header.h"
#include "mover.h"

IUnit::IUnit() {}
IUnit::IUnit(Identifier id) : m_id(id) {}

Identifier IUnit::toChar() const { return m_id; }

Unit::Unit(const Identifier& id, std::shared_ptr<IMover> mover) : IUnit(id), m_mover(mover) {}
Unit::Unit() : IUnit() {}

// Identifier Unit::toChar() const { return m_id; }

Description Unit::toString() { return Description("decription"); }

std::shared_ptr<IMover> Unit::getMover() { return m_mover; }

void Unit::setMover(std::shared_ptr<IMover> mover) { m_mover = mover; }
