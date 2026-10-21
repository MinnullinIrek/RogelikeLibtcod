#ifndef ITEMS_FACTORY_H
#define ITEMS_FACTORY_H

#include <memory>
#include <string_view>
#include <map>
#include <string>
#include <vector>

#include "eitems.h"
#include "header.h"
#include "char_types.h"
#include "items/item_structs.h"

class WearingItem;
class Armour;
class Weapon;

class ItemsFactory {
 public:
  ItemsFactory();
  ~ItemsFactory();
  std::shared_ptr<WearingItem> createWeapon(EWeaponType weaponType);
  // Создать броню по структуре (например, из readArmours()/readWeapons()).
  static std::shared_ptr<Armour> createArmour(const ArmourStruct& str);
  static std::shared_ptr<Weapon> createWeapon(const WeaponStruct& str);
  // virtual std::shared_ptr<WearingItem> createItem(int num);
  void readJson();
  // Прочитать weapons.json как готовые предметы.
  std::vector<std::shared_ptr<Weapon>> readWeaponItems() const;

 private:
  std::map<std::string, EWeaponRangedClose> m_rangedClose;
  std::map<std::string, EArmorItemTypes> m_armorTypes;
  std::map<std::string, ESetting> m_settings;
  std::map<std::string, int> m_chars;
  std::map<std::string, EWeaponSize> m_weaponSize;

  std::vector<WeaponStruct> m_strs;

 public:
  const std::vector<WeaponStruct>& weaponStructs() const { return m_strs; }

 public:
  // Прочитать armor.json и вернуть список структур брони.
  std::vector<ArmourStruct> readArmours() const;
};

// class WeaponFactory : public ItemsFactory {
//  public:
//   WeaponFactory(std::string_view jsonString = "resources/weapons.json");
//   ~WeaponFactory();
//   virtual std::shared_ptr<WearingItem> createItem(int num) override;

//  protected:
// };

#endif
