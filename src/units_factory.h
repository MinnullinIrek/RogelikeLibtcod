#ifndef UNITS_FACTORY_H
#define UNITS_FACTORY_H

#include <memory>

#include "unit_types.h"
class IUnit;
class Unit;
class UnitsFactory {
 public:
  UnitsFactory(/* args */);
  ~UnitsFactory();
 // std::shared_ptr<IUnit> UnitsFactory::createNull();
  std::shared_ptr<IUnit> createWall();

 private:
};

#endif
