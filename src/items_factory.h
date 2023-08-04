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

class ItemsFactory {
 public:
  ItemsFactory();
  ~ItemsFactory();
  std::shared_ptr<WearingItem> createWeapon(EWeaponType weaponType);
  // std::shared_ptr<WearingItem> createArmour(EArmorItemTypes armourType);
  // virtual std::shared_ptr<WearingItem> createItem(int num);
  void readJson();

 private:
  std::map<std::string, EWeaponRangedClose> m_rangedClose;
  std::map<std::string, EWearingSlot> m_slots;
  std::map<std::string, ESetting> m_settings;
  std::map<std::string, int> m_chars;
  std::map<std::string, EWeaponSize> m_weaponSize;

  std::vector<WeaponStruct> m_strs;
};

// class WeaponFactory : public ItemsFactory {
//  public:
//   WeaponFactory(std::string_view jsonString = "resources/weapons.json");
//   ~WeaponFactory();
//   virtual std::shared_ptr<WearingItem> createItem(int num) override;

//  protected:
// };

#endif
