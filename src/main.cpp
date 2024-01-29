#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__

#include <SDL.h>
#include <SDL_keycode.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>
#include <limits>

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif
#include "Bag.h"
#include "game_struct.h"
#include "info.h"
#include "items_factory.h"
#include "keyboard.h"
#include "map_generator.h"
#include "maps/map.h"
#include "units/IUnit.h"
#include "units/actor.h"
#include "units/actor_strategy_char.h"
#include "units/actor_strategy_fsm.h"
#include "units/actor_strategy_inventory.h"
#include "units/actor_strategy_map.h"
#include "units/chars.h"
#include "units/interactor.h"
#include "units/mover.h"
#include "units_factory.h"
#include "utils/consts_reader.h"
#include "utils/gamefsm.h"
#include "utils/subscriber.h"
#include "visualiser/chars_window.h"
#include "visualiser/inventory_window.h"
#include "visualiser/main_window.h"
#include "visualiser/map_window.h"
#include "visualiser/visualiser.h"
#include "visualiser/window.h"
#include "visualiser_fsm.h"

std::shared_ptr<MainWindow> mainWindow;
// GameStruct gameStruct;

/// Return the data directory.

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.

/// Game loop.
void main_loop() {
  // Rendering.

  // auto startPos(10, 10);

  // tcod::print(g_console, {0, 0}, "Hello World", TCOD_white, std::nullopt);
  gameStruct.visualiser->showMap();
  // g_context.present(g_console);

  // Handle input.
  SDL_Event event;
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_WaitEvent(&event)) {
    bool repaint = true;
    switch (event.type) {
      case SDL_QUIT:
        std::exit(EXIT_SUCCESS);
        break;
      case SDL_KEYDOWN:
        gameStruct.keyboard->setKey(event.key.keysym.sym, true);
        repaint = true;
        break;
    }
    if (true) {
      gameStruct.visualiser->showMap();
    }
  }
}

void initMapState() {
  gameStruct.actor->setStrategy(gameStruct.m_strategies.at(fsm_cxx::GameState::MapState).get());
  // gameStruct.actor->setStrategy(gameStruct.m_strategies[fsm_cxx::GameState::MapState].get());
  mainWindow->setCurrent(EMainWindows::emap);
}

void initInventoryState() {
  gameStruct.actor->setStrategy(gameStruct.m_strategies.at(fsm_cxx::GameState::InventoryState).get());
  mainWindow->setCurrent(EMainWindows::einventory);
}

void initCharState() {
  gameStruct.actor->setStrategy(gameStruct.m_strategies.at(fsm_cxx::GameState::CharState).get());
  mainWindow->setCurrent(EMainWindows::echars);
}

void prepareFsm() {
  gamefsm = new fsm_cxx::machine_t<fsm_cxx::GameState>();
  using FSM = fsm_cxx::machine_t<fsm_cxx::GameState>;  // decltype(*gamefsm);
  using States = fsm_cxx::GameState;
  // gamefsm;

  gamefsm->state().set(States::Initial).as_initial().build();
  gamefsm->state().set(States::Terminated).as_terminated().build();
  gamefsm->state()
      .set(States::Error)
      .as_error()
      .entry_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cerr << "          .. <error> entering" << '\n';
      })
      .build();
  /* gamefsm.state()
       .set(States::MapState)
       .guard([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) -> bool { return true; })
       .guard([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const& p) -> bool { return p._ok; })
       .entry_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
         std::cout << "          .. <MapState> entering" << '\n';
         initMapState();
       })
       .exit_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
         std::cout << "          .. <MapState> exiting" << '\n';
       })
       .build();*/

  // gamefsm.transition().set(fsm_cxx::GameState::Initial, fsm_cxx::begin(), fsm_cxx::GameState::MapState);
  // set(my_state::Initial, begin{}, my_state::Closed).build();
  gamefsm->transition()
      .set(fsm_cxx::GameState::Initial, events::ToBegin{}, fsm_cxx::GameState::MapState)
      .guard([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const& p) -> bool { return p._ok; })
      .entry_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> entering" << '\n';
        initMapState();
      })
      .exit_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> exiting" << '\n';
      })
      .build();

  gamefsm->transition()
      .set(fsm_cxx::GameState::MapState, events::ToInventory{}, fsm_cxx::GameState::InventoryState)
      .guard([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const& p) -> bool { return p._ok; })
      .entry_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> entering" << '\n';
        initInventoryState();
      })
      .exit_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> exiting" << '\n';
      })
      .build();

  gamefsm->transition()
      .set(fsm_cxx::GameState::InventoryState, events::ToMap{}, fsm_cxx::GameState::MapState)
      .guard([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const& p) -> bool { return p._ok; })
      .entry_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> entering" << '\n';
        initMapState();
      })
      .exit_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> exiting" << '\n';
      })
      .build();
  gamefsm->transition()
      .set(fsm_cxx::GameState::MapState, events::ToChar{}, fsm_cxx::GameState::CharState)
      .guard([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const& p) -> bool { return p._ok; })
      .entry_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> entering" << '\n';
        initCharState();
      })
      .exit_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> exiting" << '\n';
      })
      .build();
  gamefsm->transition()
      .set(fsm_cxx::GameState::CharState, events::ToChar{}, fsm_cxx::GameState::MapState)
      .guard([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const& p) -> bool { return p._ok; })
      .entry_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> entering" << '\n';
        initMapState();
      })
      .exit_action([](FSM::Event const&, FSM::Context&, FSM::State const&, FSM::Payload const&) {
        std::cout << "          .. <closed -> opened> exiting" << '\n';
      })
      .build();

  {
    // fsm_cxx::GameState::MapState;
    // ActorStrategy* strategyMove = new ActorStrategyMap();
    gameStruct.m_strategies[fsm_cxx::GameState::MapState] = std::make_unique<ActorStrategyMap>();
  }
  {
    // fsm_cxx::GameState::InventoryState;
    // ActorStrategy* strategyMove = new ActorStrategyInventory();

    gameStruct.m_strategies[fsm_cxx::GameState::InventoryState] = std::make_unique<ActorStrategyInventory>();
  }
  {
    // fsm_cxx::GameState::CharState;
    // ActorStrategy* strategyChar = new ActorStrategyChar();

    gameStruct.m_strategies[fsm_cxx::GameState::CharState] = std::make_unique<ActorStrategyChar>();
  }

  {
    // ActorStrategy* strategyFsm = new ActorStrategyFsm();

    gameStruct.m_strategyFsm = std::make_unique<ActorStrategyFsm>();
  }
  gameStruct.actor->setFsmStrategy(gameStruct.m_strategyFsm.get());

  auto st = gamefsm->state();

  gameStruct.gameFsm = gamefsm;

  gamefsm->state().set(fsm_cxx::GameState::Initial).as_initial().build();

  gamefsm->step_by(events::ToBegin{});
  // gamefsm->step_by(events::ToInventory{});
}

void initGameStruct() {
  gameStruct.unitsFactory = std::make_shared<UnitsFactory>();
  gameStruct.mapGenerator = std::make_shared<MapGenerator>(gameStruct.unitsFactory);

  gameStruct.visualiser = std::make_shared<Visualiser>(Coord(50, 50));
  gameStruct.mapGenerator->setVisualiser(gameStruct.visualiser);
  gameStruct.map = gameStruct.mapGenerator->generateRandomMap({50, 50});
  gameStruct.hero = std::static_pointer_cast<Unit>(gameStruct.unitsFactory->createHero(gameStruct.map));
  gameStruct.hero->setInteractor(std::make_shared<Interactor>());
  gameStruct.map->setHero(gameStruct.hero, {11, 11});

  gameStruct.actor = std::make_shared<Actor>();
  gameStruct.keyboard = std::make_shared<Keyboard>(gameStruct.actor);
}

/// Main program entry point.
int main(int /*argc*/, char** /*argv*/) {
  try {
    // auto params = TCOD_ContextParams{};
    // params.tcod_version = TCOD_COMPILEDVERSION;
    // params.argc = argc;
    // params.argv = argv;
    // params.renderer_type = TCOD_RENDERER_SDL2;
    // params.vsync = 1;
    // params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    // params.window_title = "Libtcod Template Project";

    // auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
    // params.tileset = tileset.get();

    // g_console = tcod::Console{80, 40};
    // params.console = g_console.get();

    // g_context = tcod::Context(params);

    MagicConsts::instance().readJson();
    initGameStruct();
    auto enemy = gameStruct.unitsFactory->createEnemy(gameStruct.map);
    gameStruct.map->setUnit(enemy, {5, 5});

    mainWindow = std::make_shared<MainWindow>(Rectangle{{1, 1}, {30, 30}});
    prepareFsm();
    // std::shared_ptr<Info> info = std::make_shared<Info>();  // todo remove
    //  info->setHero(gameStruct.hero);  // todo remove

    auto bag = gameStruct.hero->getBag();
    auto inventoryWindow = std::make_shared<InventoryWindow>(Rectangle{{1, 1}, {30, 30}});
    auto mapWindow = std::make_shared<MapWindow>(Rectangle{{1, 1}, {30, 30}});
    mainWindow->addWindow(EMainWindows::einventory, inventoryWindow);
    mainWindow->addWindow(EMainWindows::emap, mapWindow);

    gameStruct.hero->getMover()->addSubscriber(mapWindow);
    /// Albert

    gameStruct.vEffect = std::shared_ptr<VisualEffect>(new VisualEffect);
    gameStruct.vEffect->addSubscriber(mapWindow);

    bag->addSubscriber(inventoryWindow);
    bag->emit();
    gameStruct.visualiser->addWindow(std::static_pointer_cast<IWindow>(mainWindow));

    auto infoWindow = std::make_shared<Window>(
        [](std::weak_ptr<Publisher> moverPub, std::string& text, Color& color, Color& bkColor) {
          auto lockedPublisher = moverPub.lock();
          if (lockedPublisher) {
            auto mover = std::dynamic_pointer_cast<IMover>(lockedPublisher);
            if (mover) {
              auto heroCd = mover->getCoord();
              text = "Info:\n___________\n";
              text += "pos: " + heroCd.toString();
              color = {255, 255, 255};
              bkColor = {0, 0, 255};
              return 0;
            }
          }
          return 1;
        },
        std::string_view(""),
        Color{255, 255, 255},
        Color{0, 0, 255},
        Rectangle{{40, 10}, {60, 15}},
        false);
    gameStruct.hero->getMover()->addSubscriber(infoWindow);

    gameStruct.visualiser->addWindow(std::static_pointer_cast<IWindow>(infoWindow));

    auto charWindow = std::make_shared<CharsWindow>(Rectangle{{1, 1}, {30, 30}});
    auto chars = gameStruct.hero->getChars();
    mainWindow->addWindow(EMainWindows::echars, charWindow);

    std::static_pointer_cast<Publisher>(chars)->addSubscriber(charWindow);
    chars->emit();

    gameStruct.visualiser->setMap(gameStruct.map);
    // gameStruct.visualiser->setInfo(info);  // todo remove
    gameStruct.hero->lookAround(true);
    // fsm_cxx::test_state_meta();
    //  gameStruct.visualiser->setConsole(g_console);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
#else
    while (true) {
      main_loop();
    }
#endif
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << std::endl;
    throw;
  } catch (const char* exc) {
    std::cerr << exc << std::endl;
    throw;
  } catch (const std::string& exc) {
    std::cerr << exc << std::endl;
    throw;
  }
}
