
#include "visualEffect.h"

#include "../game_struct.h"
#include "../maps/cell.h"
#include "../maps/map.h"
#include "../game_struct.h"

Identifier EffectMaker::effectLoop(){

if(soloEffect.symbol.symbol != null.symbol){
counter++;
if(counter < coordCount){

 if(effectMv[counter].dirSym == DirectionMove::Up){
   effectMv[counter].cd.y--;
 }

 if(effectMv[counter].dirSym == DirectionMove::Down){
   effectMv[counter].cd.y++;
 }

 if(effectMv[counter].dirSym == DirectionMove::Right){
   effectMv[counter].cd.x++;
 }

 if(effectMv[counter].dirSym == DirectionMove::Left){
   effectMv[counter].cd.x--;
 }
 return effectMv[counter].symbol;
}
counter = -1;
}else{

  if(soloEffect.dirSym == DirectionMove::Up){
    soloEffect.cd.y--;
  }

  if(soloEffect.dirSym == DirectionMove::Down){
    soloEffect.cd.y++;
  }

  if(soloEffect.dirSym == DirectionMove::Right){
    soloEffect.cd.x++;
  }

  if(soloEffect.dirSym == DirectionMove::Left){
    soloEffect.cd.x--;
  }

 return soloEffect.symbol;
 }
return null;
}




void VisualEffect::showEffect(EffectMaker effect){
 Identifier checkIdentifier = effect.effectMv[0].symbol;


 while(checkIdentifier.symbol != null.symbol){
  checkIdentifier = effect.effectMv[0].symbol;
  //m_map->getCell()->setEffect(true,effect.effectLoop());
  //gameStruct.visualiser->showMap();
 }

}
