#include "items_factory.h"

#include <assert.h>

#include <fstream>

//#include "../json/single_include/nlohmann/json.hpp"
#include "items/IItems.h"
#include "units/chars.h"

ItemsFactory::ItemsFactory() { readJson(); }
ItemsFactory::~ItemsFactory() {}

std::shared_ptr<WearingItem> ItemsFactory::createWeapon(EWeaponType weaponType) {
  std::shared_ptr<Chars> baseChars = std::make_shared<Chars>();
  baseChars->setValue(static_cast<int>(ECharTypes::hp), 100);
  baseChars->setValue(static_cast<int>(ECharTypes::weight), 1.5);

  std::shared_ptr<Chars> damgeResistance = std::make_shared<Chars>();
  damgeResistance->setValue(static_cast<int>(EDamageType::cut), 0.9f);
  damgeResistance->setValue(static_cast<int>(EDamageType::fire), 0.5f);
  damgeResistance->setValue(static_cast<int>(EDamageType::crush), 0.1f);
  damgeResistance->setValue(static_cast<int>(EDamageType::electricity), 0.98f);
  damgeResistance->setValue(static_cast<int>(EDamageType::sound), 1.0f);
  damgeResistance->setValue(static_cast<int>(EDamageType::spike), 0.95f);
  damgeResistance->setValue(static_cast<int>(EDamageType::chop), 0.4f);

  std::shared_ptr<Chars> damage = std::make_shared<Chars>();
  damage->setValue(static_cast<int>(EDamageType::chop), 10);

  std::shared_ptr<WearingItem> witem;
  switch (weaponType) {
    case EWeaponType::axe: {
      /* code */
      WeaponStruct wpn;
      wpn.m_chars[ESetting::baseChar] = baseChars;
      wpn.m_chars[ESetting::resistance] = damgeResistance;
      wpn.m_chars[ESetting::damage] = damage;

      wpn.m_rangedClose = EWeaponRangedClose::melee;
      wpn.m_weaponSize = EWeaponSize::small;
      wpn.m_slots = {{EWearingSlot::rhand}, {EWearingSlot::lhend}};
      witem = std::make_shared<Weapon>(wpn);
    } break;
    case EWeaponType::blade:
      /* code */
      assert(false);
      break;

    default:
      assert(false);
      break;
  }
  return witem;
}

std::shared_ptr<WearingItem> ItemsFactory::createArmour(EArmorItemTypes armourType) {
  ESetting sett;
  std::shared_ptr<Chars> baseChars = std::make_shared<Chars>();
  baseChars->setValue(static_cast<int>(ECharTypes::hp), 100);
  baseChars->setValue(static_cast<int>(ECharTypes::weight), 1.5);

  std::shared_ptr<Chars> damgeResistance = std::make_shared<Chars>();
  damgeResistance->setValue(static_cast<int>(EDamageType::cut), 0.5f);
  damgeResistance->setValue(static_cast<int>(EDamageType::fire), 0.1f);
  damgeResistance->setValue(static_cast<int>(EDamageType::crush), 0.9f);
  damgeResistance->setValue(static_cast<int>(EDamageType::electricity), 0.98f);
  damgeResistance->setValue(static_cast<int>(EDamageType::sound), 1.0f);
  damgeResistance->setValue(static_cast<int>(EDamageType::spike), 0.3f);
  damgeResistance->setValue(static_cast<int>(EDamageType::chop), 0.4f);

  std::shared_ptr<Chars> damageAbsorbtion = std::make_shared<Chars>();
  damageAbsorbtion->setValue(static_cast<int>(EDamageType::chop), 0.50f);
  damageAbsorbtion->setValue(static_cast<int>(EDamageType::fire), 0.80f);
  damageAbsorbtion->setValue(static_cast<int>(EDamageType::crush), 0.30f);
  damageAbsorbtion->setValue(static_cast<int>(EDamageType::spike), 0.10f);
  damageAbsorbtion->setValue(static_cast<int>(EDamageType::cut), 0.80f);
  damageAbsorbtion->setValue(static_cast<int>(EDamageType::electricity), 0.50f);
  damageAbsorbtion->setValue(static_cast<int>(EDamageType::ice), 0.50f);
  damageAbsorbtion->setValue(static_cast<int>(EDamageType::sound), 0.0f);

  std::shared_ptr<WearingItem> witem;
  switch (armourType) {
    case EArmorItemTypes::clothes: {
      /* code */
      ArmourStruct wpn;
      wpn.m_slots = {{EWearingSlot::brest}};
      witem = std::make_shared<Armour>(wpn);
    } break;
    case EArmorItemTypes::light:
      /* code */
      assert(false);
      break;

    default:
      assert(false);
      break;
  }
  return witem;
}

void ItemsFactory::readJson() {
 /* std::ifstream fileStream("resources/weapons.json");
  nlohmann::json jsonMain;
  fileStream >> jsonMain;

  for (auto json : jsonMain) {
    auto name = json["name"];
  }*/
}
