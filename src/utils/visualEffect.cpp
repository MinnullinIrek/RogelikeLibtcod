
#include "visualEffect.h"

#include <chrono>
#include <thread>

#include "../game_struct.h"
#include "../maps/cell.h"
#include "../maps/map.h"
#include "../units/IUnit.h"
#include "../visualiser/visualiser.h"

/*
Identifier EffectMaker::effectLoop() {
  if (soloEffect.symbol.symbol != null.symbol) {
    counter++;
    if (counter < coordCount) {
      if (effectMv[counter].dirSym == DirectionMove::Up) {
        effectMv[counter].cd.y--;
      }

      if (effectMv[counter].dirSym == DirectionMove::Down) {
        effectMv[counter].cd.y++;
      }

      if (effectMv[counter].dirSym == DirectionMove::Right) {
        effectMv[counter].cd.x++;
      }

      if (effectMv[counter].dirSym == DirectionMove::Left) {
        effectMv[counter].cd.x--;
      }
      return effectMv[counter].symbol;
    }
    counter = -1;
  } else {
    if (soloEffect.dirSym == DirectionMove::Up) {
      soloEffect.cd.y--;
    }

    if (soloEffect.dirSym == DirectionMove::Down) {
      soloEffect.cd.y++;
    }

    if (soloEffect.dirSym == DirectionMove::Right) {
      soloEffect.cd.x++;
    }

    if (soloEffect.dirSym == DirectionMove::Left) {
      soloEffect.cd.x--;
    }

    return soloEffect.symbol;
  }
  return null;
}
*/

void VisualEffect::showEffect(EffectMaker effect) {
  for (auto vectCoordSymbol : effect.m_effect) {
    /// проявляем эффект
    for (auto coordSymbol : vectCoordSymbol) {
      auto cell = gameStruct.map->getCell(coordSymbol.cd);
      cell->setEffect(true, coordSymbol.id);
    }

    gameStruct.hero->getMover()->emit();
    gameStruct.visualiser->showMap();

    // todo
    /// пауза на 0.3 секунд
    // брать из json SF(...)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    /// прячем эффект
    for (auto coordSymbol : vectCoordSymbol) {
      auto cell = gameStruct.map->getCell(coordSymbol.cd);
      cell->setEffect(false, coordSymbol.id);
    }
  }
  gameStruct.visualiser->showMap();

  /*
  Identifier checkIdentifier = effect.effectMv[0].symbol;

  while (checkIdentifier.symbol != null.symbol) {
    checkIdentifier = effect.effectMv[0].symbol;
    // m_map->getCell()->setEffect(true,effect.effectLoop());
    // gameStruct.visualiser->showMap();
  }
*/
}
