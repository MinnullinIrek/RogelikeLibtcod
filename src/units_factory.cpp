#include "units_factory.h"

#include "units/IUnit.h"

UnitsFactory::UnitsFactory(/* args */) {}

UnitsFactory::~UnitsFactory() {}
/* std::shared_ptr<IUnit> UnitsFactory::createNull() {
  static std::shared_ptr<IUnit> staticNull = std::make_shared<IUnit>(nullptr, EUnitTypes::none);

  return staticNull;

}*/
std::shared_ptr<IUnit> UnitsFactory::createWall() {
  static std::shared_ptr<IUnit> staticWall = std::make_shared<IUnit>('#', EUnitTypes::wall);


  return staticWall;
}
