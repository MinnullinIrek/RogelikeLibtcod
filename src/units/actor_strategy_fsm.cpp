#include "actor_strategy_fsm.h"

#include "../fsm/fsm_cxx.hh"
#include "../game_struct.h"
#include "../utils/fsm.h"
#include "../utils/gamefsm.h"

ActorStrategyFsm::ActorStrategyFsm() {}

bool ActorStrategyFsm::doKey(EAction action) {
  // gameStruct.gameFsm;
  auto gamefsm = gameStruct.gameFsm;
  auto state = gamefsm->state();
  void* f = &gamefsm;

  switch (action) {
    case EAction::inventory:
      gamefsm->step_by(events::ToInventory{});
      return true;
      break;
    case EAction::map:
      gamefsm->step_by(events::ToMap{});
      return true;
      break;
    default:
      break;
  }

  return false;
}
