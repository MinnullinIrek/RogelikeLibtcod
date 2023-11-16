#include "actor_strategy.h"

#include "../game_struct.h"

bool ActorStrategy::executeKey(EAction action) {
  auto command = doKey(action);
  if (command) {
    gameStruct.m_invoker.setCommand(command.value());
    return true;
  }
  return false;
}
