#include "visualiser_fsm.h"
namespace fsm_cxx {

void test_state_meta() {
  fsm_cxx::machine_t<my_state> m;
  using M = decltype(m);

  // @formatter:off
  // states
  m.state().set(my_state::Initial).as_initial().build();
  m.state().set(my_state::Terminated).as_terminated().build();
  m.state()
      .set(my_state::Error)
      .as_error()
      .entry_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cerr << "          .. <error> entering" << '\n';
      })
      .build();
  m.state()
      .set(my_state::MapState)
      .guard([](M::Event const&, M::Context&, M::State const&, M::Payload const&) -> bool { return true; })
      .guard([](M::Event const&, M::Context&, M::State const&, M::Payload const& p) -> bool { return p._ok; })
      .entry_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <MapState> entering" << '\n';
      })
      .exit_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <MapState> exiting" << '\n';
      })
      .build();
  m.state()
      .set(my_state::Closed)
      .entry_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <closed> entering" << '\n';
      })
      .exit_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <closed> exiting" << '\n';
      })
      .build();

  // transitions
  m.transition().set(my_state::Initial, begin{}, my_state::Closed).build();
  m.transition()
      .set(my_state::Closed, ToMap{}, my_state::MapState)
      .guard([](M::Event const&, M::Context&, M::State const&, M::Payload const& p) -> bool { return p._ok; })
      .entry_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <closed -> MapState> entering" << '\n';
      })
      .exit_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <closed -> MapState> exiting" << '\n';
      })
      .build();
  m.transition()
      .set(my_state::MapState, close{}, my_state::Closed)
      .build()
      .transition()
      .set(my_state::Closed, end{}, my_state::Terminated)
      .build();
  m.transition()
      .set(my_state::MapState, end{}, my_state::Terminated)
      .entry_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <T><END>" << '\n';
      })
      .build();
  // @formatter:on
  m.transition()
      .set(my_state::MapState, ToInventory{}, my_state::InventoryState)
      .entry_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "from map to inventory " << '\n';
      })
      .exit_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <closed -> MapState> exiting" << '\n';
      })
      .build();

  m.on_error([](fsm_cxx::Reason reason, M::State const&, M::Context&, M::Event const&, M::Payload const&) {
    std::cout << "          Error: reason = " << reason << '\n';
  });

  // debug log
  m.on_transition(
      [&m](auto const& from, fsm_cxx::event_t const& ev, auto const& to, auto const& actions, auto const& payload) {
        std::printf(
            "        [%s] -- %s --> [%s] (payload = %s)\n",
            m.state_to_sting(from).c_str(),
            ev.to_string().c_str(),
            m.state_to_sting(to).c_str(),
            to_string(payload).c_str());
        UNUSED(actions);
      });

  // processing

  m.step_by(begin{});
  if (!m.step_by(ToMap{}, fsm_cxx::payload_t{false}))
    std::cout << "          E. cannot step to next with a false payload\n";
  m.step_by(ToMap{});
  m.step_by(close{});
  m.step_by(ToMap{});
  m.step_by(end{});

  std::printf("---- END OF test_state_meta()\n\n\n");
}

}  // namespace fsm_cxx

VisualiserFsm& VisualiserFsm::instance() {
  static VisualiserFsm visualFsm;
  return visualFsm;
}

VisualiserFsm::VisualiserFsm() {
  using M = decltype(m_fsm);
  m_fsm.state().set(fsm_cxx::my_state::Initial).as_initial().build();
  m_fsm.state().set(fsm_cxx::my_state::Terminated).as_initial().build();
  m_fsm.state()
      .set(fsm_cxx::my_state::MapState)
      .as_initial()
      .entry_action([this](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "from map to inventory " << '\n';
        emit();
        })
      .exit_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cout << "          .. <closed -> MapState> exiting" << '\n';
      })
      .build();
  m_fsm.state().set(fsm_cxx::my_state::InventoryState).as_initial().build();
  m_fsm.state()
      .set(fsm_cxx::my_state::Error)
      .as_error()
      .entry_action([](M::Event const&, M::Context&, M::State const&, M::Payload const&) {
        std::cerr << "          .. <error> entering" << '\n';
      });
}

VisualiserFsm::~VisualiserFsm() {}

void VisualiserFsm::setState(fsm_cxx::my_state state) {
  m_currentState = state;
  emit();
}

fsm_cxx::my_state VisualiserFsm::getState() { return m_currentState; }
