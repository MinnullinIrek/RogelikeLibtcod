#ifndef VISUALISERFSM_H
#define VISUALISERFSM_H

#include "fsm/fsm_cxx.hh"
#include "utils/subscriber.h"

namespace fsm_cxx {
// // namespace test {
AWESOME_MAKE_ENUM(my_state, Empty, Error, Initial, Terminated, MapState, InventoryState, CharState, Closed)

struct begin : public fsm_cxx::event_type<begin> {
  virtual ~begin() {}
  int val{9};
};
struct end : public fsm_cxx::event_type<end> {
  virtual ~end() {}
};
struct ToMap : public fsm_cxx::event_type<ToMap> {
  virtual ~ToMap() {}
};
struct ToInventory : public fsm_cxx::event_type<ToInventory> {
  virtual ~ToInventory() {}
};
struct ToChars : public fsm_cxx::event_type<ToChars> {
  virtual ~ToChars() {}
};

struct close : public fsm_cxx::event_type<close> {
  virtual ~close() {}
};
void test_state_meta();

}  // namespace fsm_cxx
// #include <fsm/fsm_cxx.hh>

// #include "utils/fsm.h"

class VisualiserFsm : public Publisher {
 public:
  VisualiserFsm& instance();
  ~VisualiserFsm();

  void setState(fsm_cxx::my_state state);

  fsm_cxx::my_state getState();

 private:
  VisualiserFsm();
  fsm_cxx::my_state m_currentState;
  fsm_cxx::machine_t<fsm_cxx::my_state> m_fsm;
};

#endif  // VISUALISERFSM_H
