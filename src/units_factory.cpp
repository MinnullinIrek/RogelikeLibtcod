#include "units_factory.h"

#include "Bag.h"
#include "helpers/corps_deleter.h"
#include "items/IItems.h"
#include "units/IUnit.h"
#include "units/chars.h"
#include "units/mover.h"

UnitsFactory::UnitsFactory(/* args */) : m_corpsDeleter(std::make_shared<CorpsDeleter>()) {}

UnitsFactory::~UnitsFactory() {}

std::shared_ptr<IUnit> UnitsFactory::createWall() {
  static std::shared_ptr<IUnit> staticWall = std::shared_ptr<IUnit>(new Unit('#', nullptr, EUnitTypes::wall));
  return staticWall;
}

std::shared_ptr<IUnit> UnitsFactory::createHero(std::shared_ptr<Map> map) {
  auto hero = std::make_shared<Unit>('@', std::static_pointer_cast<IMover>(std::make_shared<SimpleMover>(map)));
  hero->createChars();
  hero->createTestEffect();
  auto chars = hero->getChars();

  for (int i = static_cast<int>(ECharTypes::strength); i < static_cast<int>(ECharTypes::count); ++i) {
    chars->setValue(i, CharValueType(10));
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

std::shared_ptr<IUnit> UnitsFactory::createEnemy(std::shared_ptr<Map> map) {
  auto unit = std::make_shared<Unit>('E', std::static_pointer_cast<IMover>(std::make_shared<SimpleMover>(map)));
  unit->createChars();
  auto chars = unit->getChars();

  for (int i = static_cast<int>(ECharTypes::strength); i < static_cast<int>(ECharTypes::count); ++i) {
    chars->setValue(i, CharValueType(10));
  }

  // auto hp = chars->getValue(static_cast<int>(ECharTypes::hp));
  unit->setCharSubscriber(static_cast<int>(ECharTypes::hp), [unit](auto val) {
    if (val <= 0) {
      unit->die();
    }
  });
  unit->setCorpsDeleter(m_corpsDeleter);
  chars->getChar(static_cast<int>(ECharTypes::hp))->addSubscriber(unit);

  return unit;
}
