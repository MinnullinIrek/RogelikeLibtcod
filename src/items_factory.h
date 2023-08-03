#ifndef ITEMS_FACTORY_H
#define ITEMS_FACTORY_H

#include <memory>
#include <string_view>


#include "eitems.h"
#include "header.h"

class WearingItem;

class ItemsFactory {
 public:
  ItemsFactory();
  ~ItemsFactory();
  std::shared_ptr<WearingItem> createWeapon(EWeaponType weaponType);
  std::shared_ptr<WearingItem> createArmour(EArmorItemTypes armourType);
  virtual std::shared_ptr<WearingItem> createItem(int num);
  void readJson();
};

class WeaponFactory : public ItemsFactory {
 public:
  WeaponFactory(std::string_view jsonString = "resources/weapons.json");
  ~WeaponFactory();
  virtual std::shared_ptr<WearingItem> createItem(int num) override;

 protected:
};

#endif
