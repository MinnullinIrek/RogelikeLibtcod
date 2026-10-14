#include "items_factory.h"

#include <assert.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "../json/single_include/nlohmann/json.hpp"
#include "items/IItems.h"
#include "units/chars.h"

namespace {
// JSON-ключы частей тела -> слоты.
EWearingSlot slotFromJson(const std::string& key) {
  static const std::map<std::string, EWearingSlot> kSlots = {
      {"head", EWearingSlot::head},           {"neck", EWearingSlot::neck},
      {"larm", EWearingSlot::larm},           {"rarm", EWearingSlot::rarm},
      {"lforearm", EWearingSlot::lforearm},   {"rforearm", EWearingSlot::rforearm},
      {"lhand", EWearingSlot::lhand},         {"rhand", EWearingSlot::rhand},
      {"chest", EWearingSlot::chest},         {"back", EWearingSlot::back},
      {"waist", EWearingSlot::waist},         {"hips", EWearingSlot::hips},
      {"lshin", EWearingSlot::lshin},         {"rshin", EWearingSlot::rshin},
      {"lfoot", EWearingSlot::lfoot},         {"rfoot", EWearingSlot::rfoot},
  };
  auto it = kSlots.find(key);
  if (it == kSlots.end()) {
    throw std::string("unknown wearing slot in json: ") + key;
  }
  return it->second;
}

EArmorItemTypes armourTypeFromJson(const std::string& key) {
  static const std::map<std::string, EArmorItemTypes> kTypes = {
      {"clothes", EArmorItemTypes::clothes},   {"light", EArmorItemTypes::light},
      {"medium", EArmorItemTypes::medium},     {"heavy", EArmorItemTypes::heavy},
      {"exoArmour", EArmorItemTypes::exoArmour},
  };
  auto it = kTypes.find(key);
  if (it == kTypes.end()) {
    throw std::string("unknown armour type in json: ") + key;
  }
  return it->second;
}
}  // namespace

ItemsFactory::ItemsFactory() {
  m_rangedClose.insert({"close", EWeaponRangedClose::close});
  m_rangedClose.insert({"thrown", EWeaponRangedClose::thrown});
  m_rangedClose.insert({"magik", EWeaponRangedClose::magik});
  m_rangedClose.insert({"melee", EWeaponRangedClose::melee});
  m_rangedClose.insert({"ranged", EWeaponRangedClose::ranged});

  m_armorTypes.insert({"clothes", EArmorItemTypes::clothes});
  m_armorTypes.insert({"light", EArmorItemTypes::light});
  m_armorTypes.insert({"medium", EArmorItemTypes::medium});
  m_armorTypes.insert({"heavy", EArmorItemTypes::heavy});
  m_armorTypes.insert({"exoArmour", EArmorItemTypes::exoArmour});

  m_settings.insert({"abilities", ESetting::abilities});
  m_settings.insert({"addAttribute", ESetting::addAttribute});
  m_settings.insert({"addBaseChar", ESetting::addBaseChar});
  m_settings.insert({"baseChar", ESetting::baseChar});
  m_settings.insert({"damage", ESetting::damage});
  m_settings.insert({"damage1", ESetting::damage1});
  m_settings.insert({"damage2", ESetting::damage2});
  m_settings.insert({"damage3", ESetting::damage3});
  m_settings.insert({"protection", ESetting::protection});
  m_settings.insert({"resistance", ESetting::resistance});
  m_settings.insert({"skills", ESetting::skills});

  m_chars.insert({"charisma", static_cast<int>(ECharTypes::charisma)});
  m_chars.insert({"constitution", static_cast<int>(ECharTypes::constitution)});
  m_chars.insert({"dexterity", static_cast<int>(ECharTypes::dexterity)});
  m_chars.insert({"energy", static_cast<int>(ECharTypes::energy)});
  m_chars.insert({"hp", static_cast<int>(ECharTypes::hp)});
  m_chars.insert({"intelligence", static_cast<int>(ECharTypes::intelligence)});
  m_chars.insert({"manna", static_cast<int>(ECharTypes::manna)});
  m_chars.insert({"strength", static_cast<int>(ECharTypes::strength)});
  m_chars.insert({"weight", static_cast<int>(ECharTypes::weight)});
  m_chars.insert({"wisdom", static_cast<int>(ECharTypes::wisdom)});

  m_chars.insert({"chop", static_cast<int>(EDamageType::chop)});
  m_chars.insert({"crush", static_cast<int>(EDamageType::crush)});
  m_chars.insert({"cut", static_cast<int>(EDamageType::cut)});
  m_chars.insert({"electricity", static_cast<int>(EDamageType::electricity)});
  m_chars.insert({"fire", static_cast<int>(EDamageType::fire)});
  m_chars.insert({"ice", static_cast<int>(EDamageType::ice)});
  m_chars.insert({"sound", static_cast<int>(EDamageType::sound)});
  m_chars.insert({"spike", static_cast<int>(EDamageType::spike)});

  m_weaponSize.insert({"giant", EWeaponSize::giant});
  m_weaponSize.insert({"heavy", EWeaponSize::heavy});
  m_weaponSize.insert({"medium", EWeaponSize::medium});
  m_weaponSize.insert({"small", EWeaponSize::small});

  m_chars.insert({"parry", static_cast<int>(ESkills::parry)});

  readJson();
}
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

      wpn.m_rangedClose.push_back(EWeaponRangedClose::melee);
      wpn.m_weaponSize = EWeaponSize::small;
      wpn.m_slots = {{EWearingSlot::rhand}, {EWearingSlot::lhand}};
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

// std::shared_ptr<WearingItem> ItemsFactory::createArmour(EArmorItemTypes armourType) {
//   ESetting sett;
//   std::shared_ptr<Chars> baseChars = std::make_shared<Chars>();
//   baseChars->setValue(static_cast<int>(ECharTypes::hp), 100);
//   baseChars->setValue(static_cast<int>(ECharTypes::weight), 1.5);

//   std::shared_ptr<Chars> damgeResistance = std::make_shared<Chars>();
//   damgeResistance->setValue(static_cast<int>(EDamageType::cut), 0.5f);
//   damgeResistance->setValue(static_cast<int>(EDamageType::fire), 0.1f);
//   damgeResistance->setValue(static_cast<int>(EDamageType::crush), 0.9f);
//   damgeResistance->setValue(static_cast<int>(EDamageType::electricity), 0.98f);
//   damgeResistance->setValue(static_cast<int>(EDamageType::sound), 1.0f);
//   damgeResistance->setValue(static_cast<int>(EDamageType::spike), 0.3f);
//   damgeResistance->setValue(static_cast<int>(EDamageType::chop), 0.4f);

//   std::shared_ptr<Chars> damageAbsorbtion = std::make_shared<Chars>();
//   damageAbsorbtion->setValue(static_cast<int>(EDamageType::chop), 0.50f);
//   damageAbsorbtion->setValue(static_cast<int>(EDamageType::fire), 0.80f);
//   damageAbsorbtion->setValue(static_cast<int>(EDamageType::crush), 0.30f);
//   damageAbsorbtion->setValue(static_cast<int>(EDamageType::spike), 0.10f);
//   damageAbsorbtion->setValue(static_cast<int>(EDamageType::cut), 0.80f);
//   damageAbsorbtion->setValue(static_cast<int>(EDamageType::electricity), 0.50f);
//   damageAbsorbtion->setValue(static_cast<int>(EDamageType::ice), 0.50f);
//   damageAbsorbtion->setValue(static_cast<int>(EDamageType::sound), 0.0f);

//   std::shared_ptr<WearingItem> witem;
//   switch (armourType) {
//     case EArmorItemTypes::clothes: {
//       /* code */
//       ArmourStruct wpn;
//       wpn.m_slots = {{EWearingSlot::brest}};
//       witem = std::make_shared<Armour>(wpn);
//     } break;
//     case EArmorItemTypes::light:
//       /* code */
//       assert(false);
//       break;

//     default:
//       assert(false);
//       break;
//   }
//   return witem;
// }

void ItemsFactory::readJson() {
  std::string fName = "../resources/weapons.json";

  std::ifstream fileStream(fName);

  if (!fileStream.is_open()) {
    fName = "resources/weapons.json";
    fileStream.open(fName);
  }

  if (fileStream.is_open()) {
    // std::string s;
    nlohmann::json jsonMain1;
    try {
      fileStream >> jsonMain1;
    } catch (...) {
      std::cerr << "exception on reading " << fName << std::endl;
    }

    nlohmann::json jsonMain = jsonMain1.begin().value();
    // s = jsonMain.dump();
    for (auto iter = jsonMain.begin(); iter != jsonMain.end(); ++iter) {
      auto& sv = iter.value();
      WeaponStruct str;

      if (sv.contains("name")) {
        str.name = sv["name"];
      } else {
        throw std::string("weapon [name] doesn't exist in weapon.json");
      }

      if (sv.contains("description")) {
        str.description = sv["description"];
      } else {
        throw std::string("weapon [description] doesn't exist in weapon.json");
      }

      if (sv.contains("ranged")) {
        for (auto iterRanged = sv["ranged"].begin(); iterRanged != sv["ranged"].end(); ++iterRanged) {
          auto rang = iterRanged.value().get<std::string>();
          if (m_rangedClose.find(rang) == m_rangedClose.end()) {
            throw std::string("m_rangedClose doesn't contains key ") + rang;
          }
          str.m_rangedClose.push_back(m_rangedClose.at(rang));
        }
      } else {
        throw std::string("weapon [ranged] doesn't exist in weapon.json weapon name =") + str.name;
      }

      if (sv.contains("wearingSlots")) {
        for (auto iterSlots = sv["wearingSlots"].begin(); iterSlots != sv["wearingSlots"].end(); ++iterSlots) {
          std::vector<EWearingSlot> slots;
          for (auto iterSlot = iterSlots->begin(); iterSlot != iterSlots->end(); ++iterSlot) {
            auto rang = iterSlot.value().get<std::string>();
            slots.push_back(slotFromJson(rang));
          }
          str.m_slots.push_back(slots);
        }
      } else {
        throw std::string("weapon [wearingSlots] doesn't exist in weapon.json weapon name =") + str.name;
      }

      // str.m_chars;
      if (sv.contains("chars")) {
        for (auto iterChar = sv["chars"].begin(); iterChar != sv["chars"].end(); ++iterChar) {
          for (const auto& chVal : {"name", "value"}) {
            if (!iterChar.value().contains(chVal)) {
              throw std::string("weapon [chars][") + chVal + "] doesn't exist in weapon.json weapon name =" + str.name;
            }
          }
          auto& name = iterChar.value()["name"];
          ESetting sett = m_settings[name];
          std::shared_ptr<Chars> charVal = std::make_shared<Chars>();

          for (auto iterCh = iterChar.value()["value"].begin(); iterCh != iterChar.value()["value"].end(); ++iterCh) {
            auto& ch = iterCh.value();
            auto& key = ch.begin().key();
            auto val = ch.begin().value().get<CharValueType>();
            if (m_chars.find(key) == m_chars.end()) {
              throw std::string("m_rangedClose doesn't contains key ") + key;
            }
            charVal->setValue(m_chars.at(key), val);
          }

          str.m_chars.insert({sett, charVal});
        }
      } else {
        throw std::string("weapon [chars] doesn't exist in weapon.json weapon name =") + str.name;
      }

      auto key = sv["weaponSize"].get<std::string>();
      if (m_weaponSize.find(key) == m_weaponSize.end()) {
        throw std::string("m_rangedClose doesn't contains key ") + key;
      }
      str.m_weaponSize = m_weaponSize.at(key);
      m_strs.push_back(str);
    }

  } else {
    throw "file not open fName = " + fName;
  }
}

std::shared_ptr<Armour> ItemsFactory::createArmour(const ArmourStruct& str) {
  return std::make_shared<Armour>(str);
}

std::shared_ptr<Weapon> ItemsFactory::createWeapon(const WeaponStruct& str) {
  return std::make_shared<Weapon>(str);
}

std::vector<ArmourStruct> ItemsFactory::readArmours() const {
  std::vector<ArmourStruct> result;

  std::string fName = "../resources/armor.json";
  std::ifstream fileStream(fName);
  if (!fileStream.is_open()) {
    fName = "resources/armor.json";
    fileStream.open(fName);
  }
  if (!fileStream.is_open()) {
    throw "file not open fName = " + fName;
  }

  nlohmann::json jsonMain1;
  try {
    fileStream >> jsonMain1;
  } catch (...) {
    std::cerr << "exception on reading " << fName << std::endl;
  }

  nlohmann::json jsonMain = jsonMain1.begin().value();
  for (auto& sv : jsonMain) {
    ArmourStruct str;

    if (!sv.contains("name")) {
      throw std::string("armor [name] doesn't exist in armor.json");
    }
    str.name = sv["name"];

    if (sv.contains("description")) {
      str.description = sv["description"];
    }

    if (!sv.contains("armourType")) {
      throw std::string("armor [armourType] doesn't exist in armor.json armor name =") + str.name;
    }
    str.m_armourType = armourTypeFromJson(sv["armourType"].get<std::string>());

    // Части тела, которые покрывает предмет (одна или несколько).
    if (!sv.contains("wearingSlots")) {
      throw std::string("armor [wearingSlots] doesn't exist in armor.json armor name =") + str.name;
    }
    for (auto& slotJson : sv["wearingSlots"]) {
      str.m_slots.push_back(slotFromJson(slotJson.get<std::string>()));
    }

    if (sv.contains("chars")) {
      for (auto& charJson : sv["chars"]) {
        for (const auto& chVal : {"name", "value"}) {
          if (!charJson.contains(chVal)) {
            throw std::string("armor [chars][") + chVal + "] doesn't exist in armor.json armor name =" + str.name;
          }
        }
        ESetting sett = m_settings.at(charJson["name"].get<std::string>());
        std::shared_ptr<Chars> charVal = std::make_shared<Chars>();
        for (auto& ch : charJson["value"]) {
          auto& key = ch.begin().key();
          auto val = ch.begin().value().get<CharValueType>();
          if (m_chars.find(key) == m_chars.end()) {
            throw std::string("m_chars doesn't contains key ") + key;
          }
          charVal->setValue(m_chars.at(key), val);
        }
        str.m_chars.insert({sett, charVal});
      }
    }

    result.push_back(str);
  }
  return result;
}
