#include "units_factory.h"

#include "Bag.h"
#include "items/IItems.h"
#include "units/IUnit.h"
#include "units/chars.h"
#include "units/mover.h"

UnitsFactory::UnitsFactory(/* args */) {}

UnitsFactory::~UnitsFactory() {}

std::shared_ptr<IUnit> UnitsFactory::createWall() {
  static std::shared_ptr<IUnit> staticWall = std::make_shared<IUnit>('#', EUnitTypes::wall);
  return staticWall;
}

std::shared_ptr<IUnit> UnitsFactory::createHero(std::shared_ptr<Map> map) {
  auto hero = std::make_shared<Unit>('@', std::static_pointer_cast<IMover>(std::make_shared<SimpleMover>(map)));
  hero->createChars();
  auto chars = hero->getChars();

  for (int i = static_cast<int>(ECharTypes::strength); i < static_cast<int>(ECharTypes::count); ++i) {
    chars->setValue(i, CharType(10));
  }

  auto bag = hero->getBag();

  for (int i = 0; i < 30; ++i) {
    bag->putItem(
        std::make_shared<IItems>(
            "some stuff " + std::to_string(i),
            "some incredible stuff " + std::to_string(i),
            std::map<ESetting, std::shared_ptr<Chars>>{}),
        1);
  }

  return hero;
}
