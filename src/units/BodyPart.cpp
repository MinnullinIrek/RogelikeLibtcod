#include "BodyPart.h"

#include <algorithm>
#include <list>

#include <assert.h>

#include "../items/IItems.h"

Bodypart::Bodypart(EWearingSlot slot, const std::string& name, float percent, CharValueType hp)
    : m_slot(slot), m_name(name), m_percent(percent), m_hp(hp) {}

Bodypart::~Bodypart() {}

std::string Bodypart::toString() const { return m_name; }

bool Bodypart::equip(const std::weak_ptr<BodyParts>& body, std::shared_ptr<WearingItem> item) {
  if (!item) return false;

  auto armour = std::dynamic_pointer_cast<Armour>(item);
  if (armour) {
    // Разрешаем только один предмет одного типа на часть тела;
    // разные типы могут сосуществовать (одежда + легкая + средняя и т.д.).
    auto it = m_armour.find(armour->getArmourType());
    if (it != m_armour.end() && !it->second.expired() && it->second.lock() != armour) {
      return false;
    }
    m_armour[armour->getArmourType()] = armour;
    if (auto bp = body.lock()) bp->emit();
    return true;
  }

  auto weapon = std::dynamic_pointer_cast<Weapon>(item);
  if (weapon) {
    if (!isHandSlot(m_slot)) return false;
    auto cur = m_weapon.lock();
    if (cur && cur != weapon) return false;
    m_weapon = weapon;
    if (auto bp = body.lock()) bp->emit();
    return true;
  }
  return false;
}

bool Bodypart::unequip(const std::weak_ptr<BodyParts>& body, std::shared_ptr<WearingItem> item) {
  if (!item) return false;

  auto armour = std::dynamic_pointer_cast<Armour>(item);
  if (armour) {
    auto it = m_armour.find(armour->getArmourType());
    if (it != m_armour.end() && it->second.lock() == armour) {
      m_armour.erase(it);
      if (auto bp = body.lock()) bp->emit();
      return true;
    }
    return false;
  }

  auto weapon = std::dynamic_pointer_cast<Weapon>(item);
  if (weapon && m_weapon.lock() == weapon) {
    m_weapon.reset();
    if (auto bp = body.lock()) bp->emit();
    return true;
  }
  return false;
}

std::shared_ptr<Armour> Bodypart::getArmourByType(EArmorItemTypes type) const {
  auto it = m_armour.find(type);
  if (it != m_armour.end()) return it->second.lock();
  return nullptr;
}

std::string Bodypart::equippedDescription() const {
  std::string result;
  auto add = [&result](const std::string& s) {
    if (!result.empty()) result += " + ";
    result += s;
  };
  for (const auto& [type, weakArmour] : m_armour) {
    auto armour = weakArmour.lock();
    if (armour) add(std::string(armourTypeName(type)) + ": " + armour->toString());
  }
  auto weapon = m_weapon.lock();
  if (weapon) add("weapon: " + weapon->toString());
  if (result.empty()) result = "empty";
  return result;
}

//----------------- BodyParts -----------------

BodyParts::BodyParts(const std::map<EWearingSlot, std::shared_ptr<Bodypart>>& slots) : m_parts(slots) {
  rebuildIndex();
}

void BodyParts::rebuildIndex() {
  m_partsByIndex.assign(static_cast<size_t>(EWearingSlot::count), nullptr);
  for (const auto& [slot, part] : m_parts) {
    auto idx = static_cast<size_t>(slot);
    if (idx < m_partsByIndex.size()) m_partsByIndex[idx] = part;
  }
}

void BodyParts::setOwner(std::weak_ptr<Publisher> owner) { m_owner = owner; }

BodyParts::~BodyParts() {}

std::shared_ptr<Bodypart> BodyParts::getBodyPart(EWearingSlot slot) {
  auto it = m_parts.find(slot);
  if (it != m_parts.end()) {
    return it->second;
  }
  return nullptr;
}

std::shared_ptr<Bodypart> BodyParts::getBodyPart(EWearingSlot slot) const {
  auto it = m_parts.find(slot);
  if (it != m_parts.end()) {
    return it->second;
  }
  return nullptr;
}

void BodyParts::setBodyPart(EWearingSlot slot, std::shared_ptr<Bodypart> bodyPart) {
  m_parts[slot] = bodyPart;
  rebuildIndex();
}

std::vector<std::shared_ptr<Bodypart>> BodyParts::orderedParts() const {
  std::vector<std::shared_ptr<Bodypart>> result;
  for (const auto& part : m_partsByIndex) {
    if (part) result.push_back(part);
  }
  return result;
}

int BodyParts::findFirstFreeSlot(const std::shared_ptr<WearingItem>& item) const {
  if (!item || !canEquip(item)) return -1;
  // Возвращаем индекс первой покрываемой части тела — под неё ставится ">".
  for (size_t idx = 0; idx < m_partsByIndex.size(); ++idx) {
    const auto& part = m_partsByIndex[idx];
    if (!part) continue;
    for (const auto& slotVec : item->getSlots()) {
      if (std::find(slotVec.begin(), slotVec.end(), part->getSlot()) != slotVec.end()) {
        return static_cast<int>(idx);
      }
    }
  }
  return -1;
}

std::list<Text> BodyParts::showSlots(int selectedSlot) const {
  std::list<Text> result;
  for (size_t idx = 0; idx < m_partsByIndex.size(); ++idx) {
    const auto& part = m_partsByIndex[idx];
    if (!part) continue;
    std::string line = std::string(slotName(static_cast<EWearingSlot>(idx))) + ": " + part->equippedDescription();
    Color color{255, 255, 255};
    if (static_cast<int>(idx) == selectedSlot) {
      line = "> " + line;
      color = Color{255, 255, 0};
    } else if (part->equippedDescription() == "empty") {
      color = Color{130, 130, 130};
    }
    result.emplace_back(Text(line, color, Color{0, 0, 0}));
  }
  return result;
}

std::vector<std::shared_ptr<Bodypart>> BodyParts::coveredParts(const std::shared_ptr<WearingItem>& item) const {
  std::vector<std::shared_ptr<Bodypart>> result;
  if (!item) return result;
  for (const auto& slotVec : item->getSlots()) {
    for (auto slot : slotVec) {
      auto part = getBodyPart(slot);
      if (part) {
        result.push_back(part);
      }
    }
  }
  return result;
}

bool BodyParts::canEquip(const std::shared_ptr<WearingItem>& item) const {
  if (!item) return false;
  const auto& slots = item->getSlots();
  if (slots.empty()) return false;

  size_t required = 0;
  for (const auto& sv : slots) required += sv.size();
  auto parts = coveredParts(item);
  if (parts.size() != required) {
    return false;  // у юнита нет какой-то из требуемых частей тела
  }

  auto weapon = std::dynamic_pointer_cast<Weapon>(item);
  if (weapon) {
    // Оружие занимает одну или обе кисти — каждая требуемая кисть должна быть свободна
    // (или уже держать это же оружие).
    for (const auto& part : parts) {
      auto cur = part->getWeapon();
      if (cur && cur != weapon) return false;
    }
    return true;
  }

  auto armour = std::dynamic_pointer_cast<Armour>(item);
  if (armour) {
    // На каждой покрываемой части этот тип брони должен быть свободен.
    for (const auto& part : parts) {
      auto cur = part->getArmourByType(armour->getArmourType());
      if (cur && cur != armour) return false;
    }
    return true;
  }
  return false;
}

bool BodyParts::equip(const std::weak_ptr<BodyParts>& self, std::shared_ptr<WearingItem> item) {
  if (!canEquip(item)) return false;
  auto parts = coveredParts(item);
    for (auto& part : parts) {
    part->equip(self, item);
  }
  emit();
  if (auto owner = m_owner.lock()) owner->emit();
  return true;
}

bool BodyParts::unequip(const std::weak_ptr<BodyParts>& self, std::shared_ptr<WearingItem> item) {
  if (!item) return false;
  auto parts = coveredParts(item);
    bool removed = false;
  for (auto& part : parts) {
    removed = part->unequip(self, item) || removed;
  }
  if (removed) {
    emit();
    if (auto owner = m_owner.lock()) owner->emit();
  }
  return removed;
}

std::list<Text> BodyParts::showEquipped(Count count) const {
  std::list<Text> result;

  // Все экипированные уникальные предметы (один может висеть на нескольких частях тела).
  std::vector<std::shared_ptr<WearingItem>> uniqueItems;
  auto addUnique = [&uniqueItems](std::shared_ptr<WearingItem> item) {
    if (item && std::find(uniqueItems.begin(), uniqueItems.end(), item) == uniqueItems.end()) {
      uniqueItems.push_back(item);
    }
  };

  for (const auto& [slot, part] : m_parts) {
    for (const auto& [type, weakArmour] : part->getArmour()) {
      addUnique(weakArmour.lock());
    }
    addUnique(part->getWeapon());
  }

  for (const auto& item : uniqueItems) {
    // Собираем все части тела, где висит этот предмет.
    std::string where;
    for (const auto& [slot, part] : m_parts) {
      bool on = false;
      if (std::dynamic_pointer_cast<Weapon>(item)) {
        on = isHandSlot(slot) && part->getWeapon() == item;
      } else {
        auto armour = std::dynamic_pointer_cast<Armour>(item);
        for (const auto& [type, weakArmour] : part->getArmour()) {
          if (weakArmour.lock() == armour) on = true;
        }
      }
      if (on) {
        if (!where.empty()) where += "+";
        where += slotName(slot);
      }
    }

    std::string kind;
    if (auto weapon = std::dynamic_pointer_cast<Weapon>(item)) {
      kind = "weapon (" +
             std::string(weapon->getHands() == EWeaponOneTwoHand::twoHand ? "2 hand" : "1 hand") + ")";
    } else if (auto armour = std::dynamic_pointer_cast<Armour>(item)) {
      kind = std::string("armour [") + armourTypeName(armour->getArmourType()) + "]";
    } else {
      kind = "wearing";
    }
    result.emplace_back(Text(kind + ": " + item->toString() + " {" + where + "}", Color{255, 255, 255}, Color{0, 0, 0}));
  }

  while (count > 0 && static_cast<Count>(result.size()) > count) {
    result.pop_back();
  }
  return result;
}

std::shared_ptr<BodyParts> BodyParts::createHumanoid() {
  // Часть тела: имя, весовая доля (% урона/веса), hp.
  static const struct {
    EWearingSlot slot;
    const char* name;
    float percent;
    CharValueType hp;
  } kParts[] = {
      {EWearingSlot::head, "head", 0.10f, 25},       // голова
      {EWearingSlot::neck, "neck", 0.03f, 15},       // шея
      {EWearingSlot::larm, "left arm", 0.08f, 40},   // левая рука
      {EWearingSlot::rarm, "right arm", 0.08f, 40},  // правая рука
      {EWearingSlot::lforearm, "left forearm", 0.05f, 30},
      {EWearingSlot::rforearm, "right forearm", 0.05f, 30},
      {EWearingSlot::lhand, "left hand", 0.04f, 20},  // левая кисть
      {EWearingSlot::rhand, "right hand", 0.04f, 20}, // правая кисть
      {EWearingSlot::chest, "chest", 0.15f, 70},      // грудь
      {EWearingSlot::back, "back", 0.12f, 60},        // спина
      {EWearingSlot::waist, "waist", 0.05f, 30},      // пояс
      {EWearingSlot::hips, "hips", 0.07f, 40},        // бедра
      {EWearingSlot::lshin, "left shin", 0.06f, 35},  // левая голень
      {EWearingSlot::rshin, "right shin", 0.06f, 35}, // правая голень
      {EWearingSlot::lfoot, "left foot", 0.02f, 20},  // левая ступня
      {EWearingSlot::rfoot, "right foot", 0.02f, 20}, // правая ступня
  };

  std::map<EWearingSlot, std::shared_ptr<Bodypart>> parts;
  for (const auto& p : kParts) {
    parts[p.slot] = std::make_shared<Bodypart>(p.slot, p.name, p.percent, p.hp);
  }
  return std::make_shared<BodyParts>(parts);
}
