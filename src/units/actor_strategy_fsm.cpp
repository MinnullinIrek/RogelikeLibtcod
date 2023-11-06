#include "actor_strategy_fsm.h"

#include "../fsm/fsm_cxx.hh"
#include "../utils/fsm.h"
#include "../utils/gamefsm.h"

ActorStrategyFsm::ActorStrategyFsm() {}

bool ActorStrategyFsm::doKey(EAction action) {
  switch (action) {
    case EAction::inventory:
      gamefsm.step_by(events::ToInventory{});
      /* code */
      break;

    default:
      break;
  }

  return false;
}
