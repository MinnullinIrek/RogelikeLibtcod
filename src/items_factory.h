#ifndef ITEMS_FACTORY_H
#define ITEMS_FACTORY_H
#include <memory>

#include "eitems.h"
#include "header.h"

class WearingItem;

class ItemsFactory {
 public:
  ItemsFactory();
  ~ItemsFactory();
  std::shared_ptr<WearingItem> createWeapon(EWeaponType weaponType);
  std::shared_ptr<WearingItem> createArmour(EWeaponType weaponType);
};

#endif
