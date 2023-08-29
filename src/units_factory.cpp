#include "units_factory.h"
#include "units/mover.h"
#include "units/IUnit.h"

UnitsFactory::UnitsFactory(/* args */) {}

UnitsFactory::~UnitsFactory() {}

std::shared_ptr<IUnit> UnitsFactory::createWall() {
  static std::shared_ptr<IUnit> staticWall = std::make_shared<IUnit>('#', EUnitTypes::wall);
  return staticWall;
}

std::shared_ptr<IUnit> UnitsFactory::createHero(std::shared_ptr<Map> map) {
  auto hero = std::make_shared<Unit>('@', std::static_pointer_cast<IMover>(std::make_shared<SimpleMover>(map)));


  return hero;
}
