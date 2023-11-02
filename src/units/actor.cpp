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

// void Actor::move(EAction action, std::weak_ptr<Unit> whero) {
//   Coord dir = {0, 0};

//   switch (action) {
//     case EAction::down:
//       dir = {0, 1};
//       break;
//     case EAction::up:
//       dir = {0, -1};
//       break;
//     case EAction::left:
//       dir = {-1, 0};
//       break;
//     case EAction::right:
//       dir = {1, 0};
//       break;
//     case EAction::inventory:
//       // showInventoryWindow
//       break;
//     default:
//       break;
//   }

//   auto hero = whero.lock();
//   auto mover = hero->getMover();

//   hero->lookAround(false);
//   mover->moveInDirection(dir);
//   hero->lookAround(true);
//   mover->emit();
// }

void Actor::setStrategy(ActorStrategy* strategy) { m_currentStrategy = std::move(strategy); }

void Actor::setFsmStrategy(ActorStrategy* strategy) { m_fsmStrategy = std::move(strategy); }
