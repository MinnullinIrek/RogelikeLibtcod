#ifndef IITEMS_H
#define IITEMS_H
#include <map>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "../char_types.h"
#include "../eitems.h"
#include "../header.h"
#include "item_structs.h"

class Chars;

class IItems : public ToString {
 private:
  /* data */
 public:
  ItemType m_type;

  IItems(std::string_view name, std::string_view description, const std::map<ESetting, std::shared_ptr<Chars>>& chars);
  virtual ~IItems();
  virtual std::string toString() const override;
  void setChars(ESetting setting, std::shared_ptr<Chars> chars);
  std::shared_ptr<Chars> getChars(ESetting setting);

 protected:
  std::string m_name;
  std::string m_description;
  std::map<ESetting, std::shared_ptr<Chars>> m_chars;
};

class WearingItem : public IItems {
 public:
  WearingItem(const WearingStruct& wstr);
  virtual ~WearingItem();

  // Части тела, которые покрывает предмет (одежда/броня).
  const std::vector<std::vector<EWearingSlot>>& getSlots() const { return m_slots; }

 protected:
  std::vector<std::vector<EWearingSlot>> m_slots;
};

class Armour : public WearingItem {
 public:
  Armour(const ArmourStruct& str);

  virtual ~Armour();

 public:
  // Тип брони (одежда/легкая/средняя/тяжелая/экзоскелет).
  // Один и тот же тип может занимать только один слот на части тела,
  // но разные типы могут покрывать одну часть тела одновременно.
  EArmorItemTypes getArmourType() const { return m_armourType; }

 private:
  EArmorItemTypes m_armourType = EArmorItemTypes::clothes;
};

class Weapon : public WearingItem {
 public:
  Weapon(const WeaponStruct& str);

  virtual ~Weapon();

  // oneHand — weaponSlots содержит одну кисть, twoHand — обе (lhand+rhand).
  EWeaponOneTwoHand getHands() const { return m_hands; }
  EWeaponSize getSize() const { return m_weaponSize; }

 protected:
  std::vector<EWeaponType> m_weaponType;
  EWeaponSize m_weaponSize = EWeaponSize::small;
  EWeaponRangedClose m_rangedClose = EWeaponRangedClose::melee;
  EWeaponOneTwoHand m_hands = EWeaponOneTwoHand::oneHand;
};

#endif
