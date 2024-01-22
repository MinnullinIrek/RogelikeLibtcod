#ifndef VISUALEFFECT_H
#define VISUALEFFECT_H
#include <memory>
#include <vector>

#include "../header.h"

// не совсем наверное правильно в этом классе производить рассчеты
// по другому сделаем
// class Map;
/*
class EffectMaker {
 public:
  // todo
  // не надо использовать сишный массив
  //  std::vector<MoverCoord> m_effectMv;
  // если очень хочется массив, то есть std::array.
  // но лучше std::vector<MoverCoord> m_effectMv;
  MoverCoord* effectMv;
  int coordCount;

  int counter = -1;
  int dirEndPosition;
  int dirPosition;
  Identifier null;
  MoverCoord soloEffect;

  //todo перенести в cpp
  EffectMaker(int count, MoverCoord* effectMove) : coordCount(count), effectMv(effectMove){};

  //todo перенести в cpp
  EffectMaker(MoverCoord effectMove) { soloEffect = effectMove; };
  ~EffectMaker() { delete[] effectMv; };

  Identifier effectLoop();
};
*/
// давай лучше по другому

struct CoordSymbol {
  CoordSymbol(Coord&& coord, Identifier&& ident) : cd(coord), id(ident) {}
  Coord cd;
  Identifier id;
};

struct EffectMaker {
  std::vector<std::vector<CoordSymbol>> m_effect;
};

class VisualEffect {
 public:
  // сейчас не надо хранить карту внутри.
  //  сделаем по другому
  // перенести в cpp
  VisualEffect(/*std::shared_ptr<Map> map*/) = default;
  // todo
  //  хорошо придумано
  //  здесь сейчас создается новый, еще одна копия effect
  //  надо передавать либо по константной ссылке:
  /// void showEffect(const EffectMaker& effect);,
  // либо по универсальной ссылке
  /// void showEffect(EffectMaker&& effect);
  // здесь наверное лучше по универсальной ссылке: void showEffect(EffectMaker&& effect);
  void showEffect(EffectMaker effect);

 private:
  // сейчас не надо хранить карту внутри.
  //  сделаем по другому
  //  std::shared_ptr<Map> m_map;

  // todo не надо называть переменные null ))
  // m_id будет лучше
  // Identifier null;
};

#endif
