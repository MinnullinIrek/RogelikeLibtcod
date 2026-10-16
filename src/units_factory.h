#ifndef UNITS_FACTORY_H
#define UNITS_FACTORY_H

#include <memory>

#include "unit_types.h"
class IUnit;
class Unit;
class Map;
class CorpsDeleter;
// class Unit;
class UnitsFactory {
 public:
  UnitsFactory(/* args */);
  ~UnitsFactory();
  // std::shared_ptr<IUnit> UnitsFactory::createNull();
  std::shared_ptr<IUnit> createWall();
  std::shared_ptr<IUnit> createHero(std::shared_ptr<Map> map);
  // Выдать юниту части тела и положить в сумку броню (armor.json) и оружие (weapons.json).
  void equipHeroStdItems(std::shared_ptr<Unit> hero);
  std::shared_ptr<IUnit> createEnemy(std::shared_ptr<Map> map);

 private:
  std::shared_ptr<CorpsDeleter> m_corpsDeleter;
};

#endif
