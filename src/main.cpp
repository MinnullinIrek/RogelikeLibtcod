#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif  // __EMSCRIPTEN__

#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>

#if defined(_MSC_VER)
#pragma warning(disable : 4297)  // Allow "throw" in main().  Letting the compiler handle termination.
#endif
#include "maps/map.h"
#include "visualiser/visualiser.h"

std::unique_ptr<Visualiser> visualiser;

/// Return the data directory.

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.

/// Game loop.
void main_loop() {
  // Rendering.

  // auto startPos(10, 10);

  // tcod::print(g_console, {0, 0}, "Hello World", TCOD_white, std::nullopt);
  visualiser->showMap();
  //g_context.present(g_console);

  // Handle input.
  SDL_Event event;
#ifndef __EMSCRIPTEN__
  // Block until events exist.  This conserves resources well but isn't compatible with animations or Emscripten.
  SDL_WaitEvent(nullptr);
#endif
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        std::exit(EXIT_SUCCESS);
        break;
    }
  }
}

/// Main program entry point.
int main(int argc, char** argv) {
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

    auto map = std::make_unique<Map>(Coord(20, 20));
    visualiser = std::make_unique<Visualiser>(Coord(10, 10));
    visualiser->setMap(std::move(map));
    // visualiser->setConsole(g_console);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 0);
#else
    while (true) {
      main_loop();
    }
#endif
  } catch (const std::exception& exc) {
    std::cerr << exc.what() << "\n";
    throw;
  }
}
