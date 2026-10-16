#include "actor_strategy_inventory.h"

#include "../Bag.h"
#include "../game_struct.h"
#include "../items/IItems.h"
#include "../units/IUnit.h"
#include "../utils/game_log.h"
#include "../utils/game_log_message.h"
#include "command.h"

std::optional<std::shared_ptr<Command>> ActorStrategyInventory::doKey(EAction action) {
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
    case EAction::equip: {
      // Надеть выбранный предмет на части тела (броня — на свои слоты, оружие — в кисть/кисти).
      auto hero = gameStruct.hero;
      command = std::make_shared<Command>(
          [hero]() {
            if (!hero) return;
            auto item = std::dynamic_pointer_cast<WearingItem>(hero->getBag()->getSelected());
            if (hero->equipSelectedFromBag() && item) {
              gameStruct.gameLog->putMessage(GameLogMessage{0.f, "hero", "equipped: " + item->toString()});
            } else {
              gameStruct.gameLog->putMessage(GameLogMessage{0.f, "hero", "cannot equip this item"});
            }
          },
          [hero, dir]() { /* undo не поддерживается */ });
      return command;
    }
    default:
      return command;
      break;
  }
  command = std::make_shared<Command>([dir]() { gameStruct.hero->getBag()->select(dir.y); });

  return command;
}
