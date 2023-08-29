#ifndef UNITS_FACTORY_H
#define UNITS_FACTORY_H

#include <memory>

#include "unit_types.h"

class IUnit;
class Map;

class UnitsFactory {
 public:
  UnitsFactory(/* args */);
  ~UnitsFactory();
  std::shared_ptr<IUnit> createWall();
  std::shared_ptr<IUnit> createHero(std::shared_ptr<Map> map);

 private:
};

#endif
