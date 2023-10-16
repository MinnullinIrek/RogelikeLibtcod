#include "actor.h"

#include <assert.h>

#include "IUnit.h"
#include "mover.h"

Actor::Actor(std::weak_ptr<Unit> hero) : m_hero(hero) {
  assert(m_hero.lock());
  auto moveAction = [this](auto action) { move(action); };
  for (auto act : {EAction::down, EAction::left, EAction::right, EAction::up}) {
    m_actions[act] = moveAction;
  }
}

Actor::~Actor() {}

void Actor::doKey(EAction action) {
  auto hero = m_hero.lock();
  m_actions.at(action)(action);
}

void Actor::move(EAction action) {
  Coord dir = {0, 0};

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
    case EAction::inventory:
      // showInventoryWindow
      break;
    default:
      break;
  }

  auto hero = m_hero.lock();
  auto mover = hero->getMover();

  hero->lookAround(false);
  mover->moveInDirection(dir);
  hero->lookAround(true);
  mover->emit();
}
