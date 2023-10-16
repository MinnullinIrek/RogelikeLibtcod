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
#include "info.h"
#include "items_factory.h"
#include "keyboard.h"
#include "map_generator.h"
#include "maps/map.h"
#include "units/IUnit.h"
#include "units/actor.h"
#include "units/interactor.h"
#include "units/mover.h"
#include "units_factory.h"
#include "utils/consts_reader.h"
#include "utils/subscriber.h"
#include "visualiser/map_window.h"
#include "visualiser/visualiser.h"
#include "visualiser/window.h"

std::shared_ptr<Visualiser> visualiser;
std::unique_ptr<Keyboard> keyboard;

/// Return the data directory.

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.

/// Game loop.
void main_loop() {
  // Rendering.

  // auto startPos(10, 10);

  // tcod::print(g_console, {0, 0}, "Hello World", TCOD_white, std::nullopt);
  visualiser->showMap();
  // g_context.present(g_console);

  // Handle input.
  SDL_Event event;
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_WaitEvent(&event)) {
    bool repaint = false;
    switch (event.type) {
      case SDL_QUIT:
        std::exit(EXIT_SUCCESS);
        break;
      case SDL_KEYDOWN:
        keyboard->setKey(event.key.keysym.sym);
        repaint = true;
        break;
    }
    if (repaint) {
      visualiser->showMap();
    }
  }
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
    auto unitsFactory = std::make_shared<UnitsFactory>();
    auto mapGenerator = std::make_shared<MapGenerator>(unitsFactory);

    visualiser = std::make_shared<Visualiser>(Coord(50, 50));
    mapGenerator->setVisualiser(visualiser);
    auto map = mapGenerator->generateRandomMap({50, 50});
    auto hero = std::static_pointer_cast<Unit>(unitsFactory->createHero(map));
    hero->setInteractor(std::make_shared<Interactor>());
    auto itemsFactory = std::make_unique<ItemsFactory>();
    // itemsFactory->createArmour(EArmorItemTypes::clothes);
    // itemsFactory->createWeapon(EWeaponType::axe);

    map->setHero(hero, {11, 11});
    auto actor = std::make_shared<Actor>(hero);
    keyboard = std::make_unique<Keyboard>(actor);

    std::shared_ptr<Info> info = std::make_shared<Info>();
    info->setHero(hero);

    auto mapWindow = std::make_shared<MapWindow>(Rectangle{{1, 1}, {30, 30}});
    auto connector = Connector::instance();
    connector.connect(
        std::static_pointer_cast<Publisher>(hero->getMover()), std::static_pointer_cast<Subscriber>(mapWindow));

    visualiser->addWindow(std::static_pointer_cast<IWindow>(mapWindow));

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
    connector.connect(hero->getMover(), infoWindow);
    visualiser->addWindow(std::static_pointer_cast<IWindow>(infoWindow));

    visualiser->setMap(map);
    visualiser->setInfo(info);
    hero->lookAround(true);

    // visualiser->setConsole(g_console);
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
