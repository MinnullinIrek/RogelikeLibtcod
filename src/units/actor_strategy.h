#ifndef ACTOR_STRATEGY_H
#define ACTOR_STRATEGY_H

#include "../header.h"

class ActorStrategy {
 public:
  virtual bool doKey(EAction action) = 0;
};

#endif  // ACTOR_STRATEGY_H
