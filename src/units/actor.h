#ifndef ACTOR_H
#define ACTOR_H

#include <functional>
#include <memory>
#include <unordered_map>

#include "../header.h"

class Unit;
class ActorStrategy;

class Actor {
 public:
  Actor();
  ~Actor();
  void doKey(EAction action);
  void setStrategy(ActorStrategy* strategy);
  void setFsmStrategy(ActorStrategy* strategy);

 protected:
  // std::weak_ptr<Unit> m_hero;

  ActorStrategy* m_fsmStrategy = nullptr;
  ActorStrategy* m_currentStrategy = nullptr;

  // void move(EAction action, std::weak_ptr<Unit> whero);
  // std::unordered_map<EAction, std::function<void(EAction, std::weak_ptr<Unit>)>> m_actions;
};

#endif
