#include "BodyPart.h"

#include <assert.h>

#include "../items/IItems.h"
#include "../utils/random.h"

Bodypart::Bodypart(const std::string& name, float percent, CharValueType hp)
    : m_name(name), m_percent(percent), m_hp(hp) {}
Bodypart::~Bodypart() {}
std::string Bodypart::toString() const { return m_name; }
void Bodypart::setItem(std::weak_ptr<WearingItem> item) {
  bool isChanged = false;
  auto lItem = item.lock();
  if (lItem) {
    auto armour = std::dynamic_pointer_cast<Armour>(lItem);
    if (armour) {
      for (auto tp : armour->m_wearingTypes) {
        m_items[tp] = armour;
        isChanged = true;
      }
    }

    auto weapon = std::dynamic_pointer_cast<Weapon>(lItem);
    if (weapon) {
      m_weapon = weapon;
      isChanged = true;
    }
  }
  if (isChanged) {
    emit();
  }
}
void Bodypart::removeItem(std::weak_ptr<WearingItem> item) {
  bool isChanged = false;
  auto lItem = item.lock();
  if (lItem) {
    auto armour = std::dynamic_pointer_cast<Armour>(lItem);
    if (armour) {
      for (const auto& tp : armour->m_wearingTypes) {
        m_items.erase(tp);
        isChanged = true;
      }
    }
    auto weapon = std::dynamic_pointer_cast<Weapon>(lItem);
    if (m_weapon.lock() == weapon) {
      m_weapon.reset();
      isChanged = true;
    }
  }
  if (isChanged) {
    emit();
  }
}
std::weak_ptr<WearingItem> Bodypart::getWeapon() { return m_weapon; }
const std::map<EArmorItemTypes, std::weak_ptr<WearingItem>>& Bodypart::getArmour() { return m_items; }

BodyParts::BodyParts(const std::map<EWearingSlot, std::shared_ptr<Bodypart>>& slots) : m_parts(slots) {}

BodyParts::~BodyParts() {}

std::shared_ptr<Bodypart> BodyParts::getBodyPart(EWearingSlot slot) {
  if (m_parts.find(slot) != m_parts.end()) {
    return m_parts.at(slot);
  }
  return nullptr;
}

void BodyParts::setBodyPart(EWearingSlot slot, std::shared_ptr<Bodypart> bodyPart) { m_parts[slot] = bodyPart; }

std::shared_ptr<Bodypart> BodyParts::selectBpByRandom(float percent = -1) {
  if (percent == -1) {
    percent = random(1.0, 100.0);
  }
  float sumPercent = 0;
  auto last = m_parts.begin();
  for (auto it = m_parts.begin(); it != m_parts.end(); ++it) {
    sumPercent += it->second->getPercent();
    if (sumPercent > percent) {
      break;
    }
    last = it;
  }
  return last->second;
}

float Bodypart::getPercent() { return m_percent; }
CharValueType Bodypart::getHp() { return m_hp; }
void Bodypart::setHp(CharValueType hp) { m_hp = hp; }
