#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#include <memory>
#include <unordered_map>

// #include "units/actor.h"
#include "units/actor_strategy.h"
#include "units/invoker.h"
#include "utils/gamefsm.h"



class VisualEffect;
class Visualiser;
class Keyboard;
class Unit;
class Map;
class UnitsFactory;
class MapGenerator;
class Actor;
// class ActorStrategy;

struct GameStruct {
  static GameStruct& instance() {
    static GameStruct gameStr;
    return gameStr;
  }
  std::shared_ptr<Visualiser> visualiser;
  std::shared_ptr<Keyboard> keyboard;
  std::shared_ptr<Unit> hero;
  std::shared_ptr<Map> map;
  std::shared_ptr<UnitsFactory> unitsFactory;
  std::shared_ptr<MapGenerator> mapGenerator;
  std::shared_ptr<Actor> actor;
  std::unordered_map<fsm_cxx::GameState, std::unique_ptr<ActorStrategy>> m_strategies;
  std::unique_ptr<ActorStrategy> m_strategyFsm = nullptr;
  fsm_cxx::machine_t<fsm_cxx::GameState>* gameFsm;
  Invoker m_invoker;
  std::shared_ptr<VisualEffect> vEffect;
};

// static GameStruct gameStruct;
#define gameStruct GameStruct::instance()
#endif  // GAME_STRUCT_H
