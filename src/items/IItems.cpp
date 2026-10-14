#include "IItems.h"

IItems::IItems(
    std::string_view name, std::string_view description, const std::map<ESetting, std::shared_ptr<Chars>>& chars)
    : m_name(name), m_description(description), m_chars(chars), m_type(ItemType::simple) {}
IItems::~IItems() {}
std::string IItems::toString() const { return m_name; }
void IItems::setChars(ESetting setting, std::shared_ptr<Chars> chars) { m_chars[setting] = chars; }
std::shared_ptr<Chars> IItems::getChars(ESetting setting) {
  if (m_chars.find(setting) != m_chars.end()) {
    return m_chars.at(setting);
  }
  return nullptr;
}

WearingItem::WearingItem(const WearingStruct& wstr) : IItems(wstr.name, wstr.description, wstr.m_chars) {}

WearingItem::~WearingItem() {}

Armour::Armour(const ArmourStruct& str) : WearingItem(str), m_armourType(str.m_armourType) {
  // Предмет покрывает набор частей тела (одну или несколько).
  if (!str.m_slots.empty()) {
    m_slots = {str.m_slots};
  }
}

Armour::~Armour() {}

Weapon::Weapon(const WeaponStruct& str) : WearingItem(str) {
  m_slots = str.m_slots;
  m_weaponType = str.m_weaponType;
  m_weaponSize = str.m_weaponSize;
  if (!str.m_rangedClose.empty()) {
    m_rangedClose = str.m_rangedClose.front();
  }
  // Определяем одну руку занимает оружие или две по слотам кистей.
  bool hasLhand = false;
  bool hasRhand = false;
  for (const auto& slotVec : m_slots) {
    for (auto slot : slotVec) {
      if (slot == EWearingSlot::lhand) hasLhand = true;
      if (slot == EWearingSlot::rhand) hasRhand = true;
    }
  }
  m_hands = (hasLhand && hasRhand) ? EWeaponOneTwoHand::twoHand : EWeaponOneTwoHand::oneHand;
}

Weapon::~Weapon() {}
