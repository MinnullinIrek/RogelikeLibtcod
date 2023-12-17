#include "BodyPart.h"

#include <assert.h>

#include "../items/IItems.h"

Bodypart::Bodypart(const std::string& name, float percent, CharValueType hp)
    : m_name(name), m_percent(percent), m_hp(hp) {}
Bodypart::~Bodypart() {}
std::string Bodypart::toString() const { return m_name; }
void Bodypart::setItem(std::weak_ptr<WearingItem> item) {
  auto lItem = item.lock();
  if (lItem) {
    auto armour = std::dynamic_pointer_cast<Armour>(lItem);
    if (armour) {
      for (auto tp : armour->m_wearingTypes) {
        m_items[tp] = armour;
      }
    }

    auto weapon = std::dynamic_pointer_cast<Weapon>(lItem);
    if (weapon) {
      m_weapon = weapon;
    }
  }
}
void Bodypart::removeItem(std::weak_ptr<WearingItem> item) {
  auto lItem = item.lock();
  if (lItem) {
    auto armour = std::dynamic_pointer_cast<Armour>(lItem);
    if (armour) {
      for (const auto& tp : armour->m_wearingTypes) {
        m_items.erase(tp);
      }
    }
    auto weapon = std::dynamic_pointer_cast<Weapon>(lItem);
    if (m_weapon.lock() == weapon) {
      m_weapon.reset();
    }
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
