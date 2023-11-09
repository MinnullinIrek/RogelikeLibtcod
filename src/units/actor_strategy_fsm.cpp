#include "actor_strategy_fsm.h"

#include "../fsm/fsm_cxx.hh"
#include "../utils/fsm.h"
#include "../utils/gamefsm.h"
#include "../game_struct.h"

ActorStrategyFsm::ActorStrategyFsm() {}

bool ActorStrategyFsm::doKey(EAction action) {
  //gameStruct.gameFsm;
  auto gamefsm = gameStruct.gameFsm;
  auto state = gamefsm->state();
  void* f = &gamefsm;

  switch (action) {
    case EAction::inventory:
      //gameStruct
      gamefsm->step_by(events::ToInventory{});
      //gameStruct.gameFsm->step_by(events::ToBegin{});
      /* code */
      return true;
      break;

    default:
      break;
  }

  return false;
}
