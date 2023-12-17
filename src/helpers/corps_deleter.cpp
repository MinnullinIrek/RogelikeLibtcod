#include "corps_deleter.h"

#include <assert.h>

#include "../Bag.h"
#include "../game_struct.h"
#include "../maps/cell.h"
#include "../maps/map.h"
#include "../units/IUnit.h"

void CorpsDeleter::removeCorps(std::weak_ptr<IUnit> unit) {
  auto locked = unit.lock();
  auto u = std::dynamic_pointer_cast<Unit>(locked);
  int cnt = u.use_count();
  if (u) {
    auto cell = gameStruct.map->getCell(u->getCoord());
    cell->setUnit(nullptr);
    cell->getBag()->putItem(u->getBag());
  }
  cnt = u.use_count();
}
