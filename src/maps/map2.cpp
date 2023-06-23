#include "../units/IUnit.h"
#include "../units/mover.h"
#include "map.h"

void Map::setMover(std::shared_ptr<IUnit> hero, const Coord& coord) {
  auto unit = std::dynamic_pointer_cast<Unit>(hero);

  if (unit) {
    auto mover = unit->getMover();
    if (mover) {
      mover->setCoord(coord);

    }
  }
}
