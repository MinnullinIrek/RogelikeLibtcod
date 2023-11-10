#include "actor_strategy_fsm.h"



#include "../fsm/fsm_cxx.hh"
#include "../game_struct.h"
#include "../utils/fsm.h"
#include "../utils/gamefsm.h"
#include "command.h"

ActorStrategyFsm::ActorStrategyFsm() {}

std::optional<std::shared_ptr<Command>> ActorStrategyFsm::doKey(EAction action) {
  std::optional<std::shared_ptr<Command>> command;
  switch (action) {
    case EAction::inventory:
      command = std::make_shared<Command>([]() { gameStruct.gameFsm->step_by(events::ToInventory{}); });
      break;
    case EAction::map:
      command = std::make_shared<Command>([]() { gameStruct.gameFsm->step_by(events::ToMap{}); });
      break;
    default:
      break;
  }

  return command;
}
