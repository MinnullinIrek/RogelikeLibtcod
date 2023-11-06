#ifndef ACTOR_STRATEGY_Map_H
#define ACTOR_STRATEGY_Map_H

#include "actor_strategy.h"

class ActorStrategyMap : public ActorStrategy {
 public:
  virtual bool doKey(EAction action) override;
  bool operator()(EAction action) { return doKey(action); }
};

#endif  // _Map
