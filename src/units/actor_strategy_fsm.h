#ifndef ACTOR_STRATEGY_FSM_H
#define ACTOR_STRATEGY_FSM_H

#include "actor_strategy.h"

class ActorStrategyFsm : public ActorStrategy {
 public:
  ActorStrategyFsm();
  virtual bool doKey(EAction action) override;

 private:
};

#endif  // ACTOR_STRATEGY_H
