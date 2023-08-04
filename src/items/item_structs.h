#ifndef IITEMS_STRUCT_H
#define IITEMS_STRUCT_H

#include <map>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "../char_types.h"
#include "../eitems.h"
#include "../header.h"

class Chars;

struct WearingStruct {
  std::string name;
  std::string description;
  std::map<ESetting, std::shared_ptr<Chars>> m_chars;
};

struct WeaponStruct : public WearingStruct {
  std::vector<std::vector<EWearingSlot>> m_slots;
  std::vector<EWeaponType> m_weaponType;
  EWeaponSize m_weaponSize;
  std::vector<EWeaponRangedClose> m_rangedClose;
};

struct ArmourStruct : public WearingStruct {
  std::vector<std::vector<EWearingSlot>> m_slots;
  std::vector<EArmorItemTypes> m_armourType;
  std::map<ESetting, std::shared_ptr<Chars>> m_chars;
};

#endif
