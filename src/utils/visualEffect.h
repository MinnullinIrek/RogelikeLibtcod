#ifndef VISUALEFFECT_H
#define VISUALEFFECT_H
#include "../header.h"
#include <memory>

class Map;

class EffectMaker{
public:
 MoverCoord *effectMv;
 int coordCount;
 int counter = -1;
 int dirEndPosition;
 int dirPosition;
 Identifier null;
  MoverCoord soloEffect;


 EffectMaker(int count, MoverCoord *effectMove)
 : coordCount(count), effectMv(effectMove){};

 EffectMaker(MoverCoord effectMove){soloEffect = effectMove;};
 ~EffectMaker(){delete[] effectMv;};

 Identifier effectLoop();

};

class VisualEffect{
  public:
  VisualEffect(std::shared_ptr<Map> map);
  void showEffect(EffectMaker effect);


  private:
  std::shared_ptr<Map> m_map;
  Identifier null;
  };

#endif
