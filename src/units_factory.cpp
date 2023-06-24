#include "units_factory.h"

#include "units/IUnit.h"

UnitsFactory::UnitsFactory(/* args */) {}

UnitsFactory::~UnitsFactory() {}

std::shared_ptr<IUnit> UnitsFactory::createWall() {
  static std::shared_ptr<IUnit> staticWall = std::make_shared<IUnit>('#');
  return staticWall;
}
