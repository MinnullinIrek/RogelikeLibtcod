#include "IUnit.h"

#include "../Bag.h"
#include "../header.h"
#include "../maps/cell.h"
#include "../maps/map.h"
#include "../utils/consts_reader.h"
#include "algorithm"
#include "chars.h"
#include "interactor.h"
#include "mover.h"
#include "effect.h"

IUnit::IUnit() : m_type(EUnitTypes::none) {}
IUnit::IUnit(Identifier id, EUnitTypes uType) : m_id(id), m_type(uType) {
  TestEffect* effect = new TestEffect();
  effect->m_attacker = weak_from_this();
  
  m_effectProtoType.reset((Effect*)effect);
}
void IUnit::createChars() { m_chars = std::make_shared<Chars>(); }
IUnit::~IUnit() {}
void IUnit::setInteractor(std::shared_ptr<Interactor> interactor) { m_currentInteractor = interactor; }
std::shared_ptr<Interactor> IUnit::getInteractor() { return m_currentInteractor; }

std::shared_ptr<Chars> IUnit::getChars() { return m_chars; }

Identifier IUnit::toChar() const { return m_id; }
std::unique_ptr<Effect> IUnit::getEffect() { return m_effectProtoType->clone(); }

void IUnit::acceptEffect(std::unique_ptr<Effect> effect) { effect->visit(weak_from_this()); }


Unit::Unit(const Identifier& id, std::shared_ptr<MoverInterface> mover)
    : IUnit(id, EUnitTypes::none), m_mover(mover), m_bag(std::make_shared<Bag>()) {}
Unit::Unit() : IUnit() {}

// Identifier Unit::toChar() const { return m_id; }

Description Unit::toString() const { return Description("decription"); }

std::shared_ptr<MoverInterface> Unit::getMover() { return m_mover; }

void Unit::setMover(std::shared_ptr<MoverInterface> mover) { m_mover = mover; }

void IUnit::setBodyParts(std::shared_ptr<BodyParts> bp) { m_bodyParts = bp; }

std::shared_ptr<BodyParts> IUnit::getBodyParts() { return m_bodyParts; }

void Unit::lookAround(bool isEyeOpened) {
  if (isEyeOpened) {
    auto watchingLength = m_chars->getValue(static_cast<int>(ECharTypes::perception)) / 2;
    if (watchingLength == 0) {
      // throw "person is blind";
      watchingLength = 5;
    }

    const auto& heroCoord = m_mover->getCoord();
    auto map = m_mover->getMap().lock();

    Coord cd = heroCoord;
    std::initializer_list<Coord> tempCoords{{-1, 1}, {-1, 0}, {-1, -1}, {0, 1}, {0, -1}, {1, 1}, {1, 0}, {1, -1}};

    auto isFar = [&heroCoord](const Coord& lastCd, const Coord& nextCd) -> bool {
      return ((lastCd - heroCoord).length() + SF("visibilityMagic")) <= (nextCd - heroCoord).length();
    };

    m_watchingCoords.clear();
    m_watchingCoords[cd] = true;
    map->setSeen(cd);

    std::list<Coord> checkingCoords;
    checkingCoords.push_back(heroCoord);

    for (int i = 0; i < watchingLength; ++i) {
      for (auto it = checkingCoords.begin(); it != checkingCoords.end();) {
        auto wCd = *it;
        it = checkingCoords.erase(it);

        if (!map->isWall(wCd)) {
          for (const auto& ct : tempCoords) {
            auto cdTemp = wCd + ct;
            if (isFar(wCd, cdTemp)) {
              m_watchingCoords[cdTemp] = true;
              map->setSeen(cdTemp);
              if ((heroCoord - cdTemp).length() <= watchingLength) {
                checkingCoords.push_front(cdTemp);
              }
            }
          }
        }
      }
    }
  } else {
    m_watchingCoords.clear();
  }
  getMover()->emit();
}

const std::unordered_map<Coord, bool, KeyHasher>& Unit::getWatchingCoords() const { return m_watchingCoords; }

std::shared_ptr<Bag> Unit::getBag() { return m_bag; }

void Unit::moveInDirection(const Coord& coord) {
  if (m_mover) {
    m_mover->moveInDirection(coord);
  }
}

void Unit::setCoord(const Coord& currentPos) {
  if (m_mover) {
    m_mover->setCoord(currentPos);
  }
}
const Coord& Unit::getCoord() const {
  if (m_mover) {
    return m_mover->getCoord();
  }
  return {0, 0};
}

std::weak_ptr<Map> Unit::getMap() {
  if (m_mover) {
    return m_mover->getMap();
  }
  return std::shared_ptr<Map> (nullptr);
}
void Unit::changeMap(std::weak_ptr<Map> map) {
  if (m_mover) {
    m_mover->changeMap(map);
  }
}
