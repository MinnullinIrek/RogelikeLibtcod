#ifndef ACTOR_STRATEGY_INVENTORY_H
#define ACTOR_STRATEGY_INVENTORY_H

#include "actor_strategy.h"

class ActorStrategyInventory : public ActorStrategy {
 public:
  virtual std::optional<std::shared_ptr<Command>> doKey(EAction action) override;
};

#endif  // INVENTORY
