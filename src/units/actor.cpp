#include "actor.h"

#include <assert.h>

#include "../game_struct.h"
#include "IUnit.h"
#include "actor_strategy.h"
#include "mover.h"

Actor::Actor() {
  // auto moveAction = [this](auto action, std::weak_ptr<Unit> hero) { move(action, hero); };
  // for (auto act : {EAction::down, EAction::left, EAction::right, EAction::up}) {
  //   m_actions[act] = moveAction;
  // }
}

Actor::~Actor() {}

void Actor::doKey(EAction action) {
  if (action == EAction::undo) {
    gameStruct.m_invoker.undo();
    return;
  }

  bool isFsm = false;
  if (m_fsmStrategy) {
    isFsm = m_fsmStrategy->executeKey(action);
  }
  if (!isFsm && m_currentStrategy) {
    auto command = m_currentStrategy->executeKey(action);
  }
  gameStruct.m_invoker.run();
}

void Actor::setStrategy(ActorStrategy* strategy) { m_currentStrategy = std::move(strategy); }

void Actor::setFsmStrategy(ActorStrategy* strategy) { m_fsmStrategy = std::move(strategy); }
