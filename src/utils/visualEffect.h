#ifndef VISUALEFFECT_H
#define VISUALEFFECT_H
#include <future>
#include <memory>
#include <vector>

#include "../header.h"
#include "../utils/subscriber.h"


struct CoordSymbol {
  CoordSymbol(Coord&& coord, Identifier&& ident) : cd(coord), id(ident) {}
  Coord cd;
  Identifier id;
};

struct EffectMaker {
  std::vector<std::vector<CoordSymbol>> m_effect;
  std::launch m_asyncType = std::launch::deferred;
};

class VisualEffect : public Publisher {
 public:
  void showEffect(const EffectMaker& effect);
  void effectStart(const EffectMaker& effect);
  void setCurrentState(std::vector<std::vector<CoordSymbol>>::const_iterator state);
  std::future<void> fut;
  std::vector<std::vector<CoordSymbol>>::const_iterator m_currentState;
};

#endif
