#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#include <memory>
#include <unordered_map>

// #include "units/actor.h"
#include "utils/gamefsm.h"

class Visualiser;
class Keyboard;
class Unit;
class Map;
class UnitsFactory;
class MapGenerator;
class Actor;
class ActorStrategy;

struct GameStruct {
  std::shared_ptr<Visualiser> visualiser;
  std::shared_ptr<Keyboard> keyboard;
  std::shared_ptr<Unit> hero;
  std::shared_ptr<Map> map;
  std::shared_ptr<UnitsFactory> unitsFactory;
  std::shared_ptr<MapGenerator> mapGenerator;
  std::shared_ptr<Actor> actor;
  std::unordered_map<fsm_cxx::GameState, std::unique_ptr<ActorStrategy>> m_strategies;
};

static GameStruct gameStruct;
#endif  // GAME_STRUCT_H
