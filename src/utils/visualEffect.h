#ifndef VISUALEFFECT_H
#define VISUALEFFECT_H
#include <memory>
#include <vector>
#include "../utils/subscriber.h"

#include "../header.h"

struct CoordSymbol {
  CoordSymbol(Coord&& coord, Identifier&& ident) : cd(coord), id(ident) {}
  Coord cd;
  Identifier id;
};

struct EffectMaker {
  std::vector<std::vector<CoordSymbol>> m_effect;
};

class VisualEffect : public Publisher {
 public:
  void showEffect(const EffectMaker& effect);
  void setCurrentState(std::vector<std::vector<CoordSymbol>>::const_iterator state);

 public:
  std::vector<std::vector<CoordSymbol>>::const_iterator m_currentState;
};

#endif
