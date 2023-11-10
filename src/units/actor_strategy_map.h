#ifndef ACTOR_STRATEGY_Map_H
#define ACTOR_STRATEGY_Map_H

#include "actor_strategy.h"

class ActorStrategyMap : public ActorStrategy {
 public:
  virtual std::optional<std::shared_ptr<Command>> doKey(EAction action) override;
};

#endif  // _Map
