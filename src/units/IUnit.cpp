#include "IUnit.h"

#include "../Bag.h"
#include "../header.h"
#include "algorithm"
#include "chars.h"
#include "interactor.h"
#include "mover.h"

IUnit::IUnit() {}
IUnit::IUnit(Identifier id, EUnitTypes uType) : m_id(id), m_type(uType) {}
void IUnit::createChars() { m_chars = std::make_shared<Chars>(); }
IUnit::~IUnit() {}
void IUnit::setInteractor(std::shared_ptr<Interactor> interactor) { m_currentInteractor = interactor; }
std::shared_ptr<Interactor> IUnit::getInteractor() { return m_currentInteractor; }

std::shared_ptr<Chars> IUnit::getChars() { return m_chars; }

Identifier IUnit::toChar() const { return m_id; }

Unit::Unit(const Identifier& id, std::shared_ptr<IMover> mover) : IUnit(id), m_mover(mover) {}
Unit::Unit() : IUnit() {}

// Identifier Unit::toChar() const { return m_id; }

Description Unit::toString() const { return Description("decription"); }

std::shared_ptr<IMover> Unit::getMover() { return m_mover; }

void Unit::setMover(std::shared_ptr<IMover> mover) { m_mover = mover; }

void IUnit::setBodyParts(std::shared_ptr<BodyParts> bp) { m_bodyParts = bp; }

std::shared_ptr<BodyParts> IUnit::getBodyParts() { return m_bodyParts; }
