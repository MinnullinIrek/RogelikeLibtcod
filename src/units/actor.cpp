#include "actor.h"

#include <assert.h>

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
  if (m_fsmStrategy) {
    if (m_fsmStrategy->doKey(action)) {
      return;
    }
  }
  if (m_currentStrategy) {
    m_currentStrategy->doKey(action);
  }
}

void Actor::setStrategy(ActorStrategy* strategy) { m_currentStrategy = std::move(strategy); }

void Actor::setFsmStrategy(ActorStrategy* strategy) { m_fsmStrategy = std::move(strategy); }
