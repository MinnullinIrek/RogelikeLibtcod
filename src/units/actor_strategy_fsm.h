#ifndef ACTOR_STRATEGY_FSM_H
#define ACTOR_STRATEGY_FSM_H

#include "actor_strategy.h"

class ActorStrategyFsm : public ActorStrategy {
 public:
  virtual bool doKey(EAction action);
};

#endif  // ACTOR_STRATEGY_H
