#include "actor_strategy_map.h"

#include "../game_struct.h"
#include "../units/IUnit.h"
#include "../units/mover.h"
#include "command.h"

std::optional<std::shared_ptr<Command>> ActorStrategyMap::doKey(EAction action) {
  // auto hero = gameStruct.hero;
  Coord dir = {0, 0};
  std::optional<std::shared_ptr<Command>> command;
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
      return command;
      break;
  }
  command = std::make_shared<Command>(
      [dir]() {
        auto hero = gameStruct.hero;
        auto mover = hero->getMover();
        hero->lookAround(false);
        mover->moveInDirection(dir);
        hero->lookAround(true);
        mover->emit();
      },
      [dir]() {
        auto hero = gameStruct.hero;
        auto mover = hero->getMover();
        hero->lookAround(false);
        mover->moveInDirection(dir.revert());
        hero->lookAround(true);
        mover->emit();
      });
  return command;
}
