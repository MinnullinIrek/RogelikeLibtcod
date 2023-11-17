#ifndef GAMEFSM_H
#define GAMEFSM_H

#include "../fsm/fsm_cxx.hh"

namespace fsm_cxx {
AWESOME_MAKE_ENUM(GameState, Empty, Error, Initial, Terminated, MapState, InventoryState, CharState, Closed);
}  // namespace fsm_cxx

struct events {
  struct ToBegin : public fsm_cxx::event_type<ToBegin> {
    virtual ~ToBegin() {}
    int val{9};
  };
  struct ToEnd : public fsm_cxx::event_type<ToEnd> {
    virtual ~ToEnd() {}
  };
  struct ToMap : public fsm_cxx::event_type<ToMap> {
    virtual ~ToMap() {}
  };
  struct ToChar : public fsm_cxx::event_type<ToChar> {
    virtual ~ToChar() {}
  };
  struct ToInventory : public fsm_cxx::event_type<ToInventory> {
    virtual ~ToInventory() {}
  };
  struct ToClose : public fsm_cxx::event_type<ToClose> {
    virtual ~ToClose() {}
  };
};

static fsm_cxx::machine_t<fsm_cxx::GameState>* gamefsm;

#endif  // GAMEFSM_H
