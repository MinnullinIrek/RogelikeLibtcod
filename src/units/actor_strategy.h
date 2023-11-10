#ifndef ACTOR_STRATEGY_H
#define ACTOR_STRATEGY_H

#include "../header.h"
#include <optional>

class Command;

class ActorStrategy {
 public:
  virtual std::optional<std::shared_ptr<Command>> doKey(EAction action) = 0;
};

#endif  // ACTOR_STRATEGY_H
