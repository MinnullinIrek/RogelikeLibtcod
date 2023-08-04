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

 protected:
  std::vector<std::vector<EWearingSlot>> m_slots;
};

class Armour : public WearingItem {
 public:
  Armour(const ArmourStruct& str);

  virtual ~Armour();

 public:
  std::vector<EArmorItemTypes> m_wearingTypes;
};

class Weapon : public WearingItem {
 public:
  Weapon(const WeaponStruct& str);

  virtual ~Weapon();

 protected:
  std::vector<EWeaponType> m_weaponType;
  EWeaponSize m_weaponSize;
  EWeaponRangedClose m_rangedClose;
};

#endif
