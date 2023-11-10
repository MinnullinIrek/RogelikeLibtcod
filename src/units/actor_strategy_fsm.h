#ifndef ACTOR_STRATEGY_FSM_H
#define ACTOR_STRATEGY_FSM_H

#include <optional>

#include "actor_strategy.h"


class ActorStrategyFsm : public ActorStrategy {
 public:
  ActorStrategyFsm();
  virtual std::optional<std::shared_ptr<Command>> doKey(EAction action) override;

 private:
};

#endif  // ACTOR_STRATEGY_H
