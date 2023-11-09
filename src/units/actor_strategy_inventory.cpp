#include "actor_strategy_inventory.h"

#include "../Bag.h"
#include "../game_struct.h"
#include "../units/IUnit.h"
#include "../units/mover.h"

bool ActorStrategyInventory::doKey(EAction action) {
  auto hero = gameStruct.hero;
  auto bag = hero->getBag();
  Coord dir = {0, 0};

  switch (action) {
    case EAction::down:
      dir = {0, 1};
      break;
    case EAction::up:
      dir = {0, -1};
      break;
    case EAction::left:
      dir = {-1, 0};
      break;
    case EAction::right:
      dir = {1, 0};
      break;
    default:
      return false;
      break;
  }
  auto mover = hero->getMover();
  hero->lookAround(false);
  mover->moveInDirection(dir);
  hero->lookAround(true);
  mover->emit();

  return false;
}
