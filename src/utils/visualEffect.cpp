
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
  //fut = std::async(
      //std::launch::async,
      //[this, effect]() {  /// Albert вместо std::launch::deferred effect.laun, effect- надо отправлять не по ссылке для
                           /// асинхронной работы effect без амперсанда
        for (std::vector<std::vector<CoordSymbol>>::const_iterator it = effect.m_effect.begin();
             it != effect.m_effect.end();
             ++it) {
          /// проявляем эффект

          setCurrentState(it);
          gameStruct.hero->getMover()->emit();
          //  fut.get(); /// Albert перенести ниже
        }
     //)};

  // if(effect.laun) { // если блокирует(работает в 1м потоке)
  // fut.get(); //тут же и выполнить
  // }
}

void VisualEffect::effectStart(const EffectMaker& effect){
  if(effect.laun == std::launch::async){
    fut = std::async(effect.laun ,[this,effect](){
      showEffect(effect);
    });
  }
  else if(effect.laun == std::launch::deferred){
    showEffect(effect);
  }
}


void VisualEffect::setCurrentState(std::vector<std::vector<CoordSymbol>>::const_iterator state) {
  m_currentState = state;
  emit();
  gameStruct.visualiser->showMap();
  std::this_thread::sleep_for(std::chrono::milliseconds(300));  // 300ms => 0.3sek
}
