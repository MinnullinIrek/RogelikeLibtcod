#ifndef ACTOR_STRATEGY_CHAR_H
#define ACTOR_STRATEGY_CHAR_H

#include "actor_strategy.h"

class ActorStrategyChar : public ActorStrategy {
 public:
  virtual std::optional<std::shared_ptr<Command>> doKey(EAction action) override;
};

#endif  // CHAR
