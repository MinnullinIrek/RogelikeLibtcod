#ifndef ACTOR_H
#define ACTOR_H

#include <memory>
#include <unordered_map>
#include <functional>
#include "../header.h"

class Unit;

class Actor {
 private:
  /* data */
 public:
  Actor(std::weak_ptr<Unit> hero);
  ~Actor();
  void doKey(EAction action);
  void move(EAction action);

 protected:
  std::weak_ptr<Unit> m_hero;
  std::unordered_map<EAction, std::function<void(EAction)>> m_actions;
};

#endif
