
#include "visualEffect.h"

#include <chrono>
#include <thread>

#include "../game_struct.h"
#include "../maps/cell.h"
#include "../maps/map.h"
#include "../units/IUnit.h"
#include "../visualiser/visualiser.h"
#include "consts_reader.h"

void VisualEffect::showEffect(const EffectMaker& effect) {
  for (std::vector<std::vector<CoordSymbol>>::const_iterator it = effect.m_effect.begin(); it != effect.m_effect.end();
       ++it) {
    /// проявляем эффект

    setCurrentState(it);
    // gameStruct.hero->getMover()->emit(); // Albert раскоментировать прячем устаревший кадр эффектов
  }
}

void VisualEffect::setCurrentState(std::vector<std::vector<CoordSymbol>>::const_iterator state) {
  m_currentState = state;
  emit();
  gameStruct.visualiser->showMap();
  std::this_thread::sleep_for(std::chrono::milliseconds(SI("effectDuration")));  // 300ms => 0.3sek
}

void VisualEffect::showWindowEffect() {  /// ALbert удалить функцию
  // emit();

  gameStruct.visualiser->showMap();
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
}
