#ifndef ACTOR_STRATEGY_H
#define ACTOR_STRATEGY_H

#include <optional>

#include "../header.h"

class Command;

class ActorStrategy {
 public:
  bool executeKey(EAction action);

 protected:
  virtual std::optional<std::shared_ptr<Command>> doKey(EAction action) = 0;
};

#endif  // ACTOR_STRATEGY_H
