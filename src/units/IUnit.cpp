#include "IUnit.h"

#include "../Bag.h"
#include "../header.h"
#include "../helpers/corps_deleter.h"
#include "../items/IItems.h"
#include "../maps/cell.h"
#include "../maps/map.h"
#include "../utils/consts_reader.h"
#include "BodyPart.h"
#include "algorithm"
#include "chars.h"
#include "effect.h"
#include "interactor.h"
#include "mover.h"

const Coord zeroCoord = Coord{0, 0};

IUnit::IUnit() {}
void Unit::createChars() { m_chars = std::make_shared<Chars>(); }
IUnit::~IUnit() {}

Unit::~Unit() {}
void Unit::setInteractor(std::shared_ptr<Interactor> interactor) { m_currentInteractor = interactor; }
std::shared_ptr<Interactor> Unit::getInteractor() { return m_currentInteractor; }
std::shared_ptr<Chars> Unit::getChars() { return m_chars; }

Identifier Unit::toChar() const { return m_id; }
std::unique_ptr<Effect> Unit::getEffect() { return m_effectProtoType->clone(); }

void Unit::acceptEffect(std::unique_ptr<Effect> effect) {
  effect->visit(std::dynamic_pointer_cast<IUnit>(shared_from_this()));
}

Unit::Unit(const Identifier& id, std::shared_ptr<MoverInterface> mover, EUnitTypes type)
    : IUnit(), m_id(id), m_type(type), m_mover(mover), m_bag(std::make_shared<Bag>()) {}

void Unit::createTestEffect() {
  TestEffect* effect = new TestEffect();
  effect->m_attacker = std::dynamic_pointer_cast<IUnit>(shared_from_this());
  m_effectProtoType.reset((Effect*)effect);
}

Unit::Unit() : IUnit() {}

Description Unit::toString() const { return Description("decription"); }

std::shared_ptr<MoverInterface> Unit::getMover() { return m_mover; }

void Unit::setMover(std::shared_ptr<MoverInterface> mover) { m_mover = mover; }

void Unit::setBodyParts(std::shared_ptr<BodyParts> bp) {
  m_bodyParts = bp;
  if (m_bodyParts) {
    // Владелец частей тела — юнит: экипировка/снятие каскадно уведомляет подписчиков.
    m_bodyParts->setOwner(weak_from_this());
  }
}

bool Unit::equipSelectedFromBag() {
  if (!m_bodyParts || !m_bag) return false;
  auto item = std::dynamic_pointer_cast<WearingItem>(m_bag->getSelected());
  if (!item) return false;
  if (!m_bodyParts->canEquip(item)) return false;

  // Вешаем предмет на части тела (они хранят weak-ссылки, strong — у юнита),
  // затем убираем его из сумки.
  if (!m_bodyParts->equip(m_bodyPartsWeak(), item)) return false;
  m_equippedItems.push_back(item);
  m_equippedSelected = static_cast<int>(m_equippedItems.size()) - 1;
  m_bag->popItem();
  emit();
  return true;
}

void Unit::selectEquipped(int delta) {
  if (m_equippedItems.empty()) return;
  int size = static_cast<int>(m_equippedItems.size());
  if (m_equippedSelected < 0)
    m_equippedSelected = 0;
  else
    m_equippedSelected = (m_equippedSelected + delta + size) % size;
  emit();
}

std::shared_ptr<WearingItem> Unit::getEquippedSelectedItem() const {
  if (m_equippedSelected < 0 || m_equippedSelected >= static_cast<int>(m_equippedItems.size())) return nullptr;
  return m_equippedItems[m_equippedSelected];
}

int Unit::getHighlightedSlotIndex() const {
  if (!m_bodyParts || !m_bag) return -1;
  auto item = std::dynamic_pointer_cast<WearingItem>(m_bag->getSelected());
  if (!item) return -1;
  return m_bodyParts->findFirstFreeSlot(item);
}

bool Unit::unequipSelected() {
  auto item = getEquippedSelectedItem();
  if (!item) return false;
  bool ok = unequipToBag(item);
  if (ok && m_equippedSelected >= static_cast<int>(m_equippedItems.size())) {
    m_equippedSelected = static_cast<int>(m_equippedItems.size()) - 1;
  }
  return ok;
}

bool Unit::unequipToBag(const std::shared_ptr<WearingItem>& item) {
  if (!m_bodyParts || !item) return false;
  if (!m_bodyParts->unequip(m_bodyPartsWeak(), item)) return false;
  // Возвращаем strong-ссылку с тела в сумку.
  auto it = std::find(m_equippedItems.begin(), m_equippedItems.end(), item);
  if (it != m_equippedItems.end()) {
    int idx = static_cast<int>(it - m_equippedItems.begin());
    m_equippedItems.erase(it);
    if (m_equippedSelected >= idx) m_equippedSelected = std::max(-1, m_equippedSelected - 1);
  }
  if (m_bag) {
    // Снятую вещь кладём в сумку как новый предмет.
    m_bag->putItem(item);
  }
  emit();
  return true;
}

std::shared_ptr<BodyParts> Unit::getBodyParts() { return m_bodyParts; }

std::weak_ptr<BodyParts> Unit::m_bodyPartsWeak() const { return m_bodyParts; }

void Unit::lookAround(bool isEyeOpened) {
  if (isEyeOpened) {
    auto watchingLength = m_chars->getValue(static_cast<int>(ECharTypes::perception)) / 2;
    if (watchingLength == 0) {
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
  return zeroCoord;
}

std::weak_ptr<Map> Unit::getMap() {
  if (m_mover) {
    return m_mover->getMap();
  }
  return std::shared_ptr<Map>(nullptr);
}
void Unit::changeMap(std::weak_ptr<Map> map) {
  if (m_mover) {
    m_mover->changeMap(map);
  }
}

EUnitTypes Unit::getType() { return m_type; }

void Unit::notify(std::weak_ptr<Publisher> publisher) {
  auto locked = publisher.lock();
  if (locked) {
    auto charVal = std::dynamic_pointer_cast<Char>(locked);
    if (charVal) {
      if (m_charSubscribers.find(charVal->m_charType) != m_charSubscribers.end()) {
        m_charSubscribers.at(charVal->m_charType)(charVal->getValue());
      }
    }
  }
}

void Unit::setCharSubscriber(int charType, CharSubscriber&& charSub) { m_charSubscribers[charType] = charSub; }

void Unit::die() {
  auto corpsDeleter = m_corpsDeleter.lock();
  if (corpsDeleter) {
    corpsDeleter->removeCorps(std::dynamic_pointer_cast<Unit>(shared_from_this()));
  }
}

void Unit::setCorpsDeleter(std::shared_ptr<CorpsDeleter> deleter) { m_corpsDeleter = deleter; }
