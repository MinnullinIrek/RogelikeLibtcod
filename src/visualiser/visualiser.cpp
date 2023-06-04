#include "visualiser.h"

#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "../maps/map.h"

auto get_data_dir() -> std::filesystem::path {
  static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
  while (!std::filesystem::exists(root_directory / "data")) {
    // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
    root_directory /= "..";
    if (!std::filesystem::exists(root_directory)) {
      throw std::runtime_error("Could not find the data directory.");
    }
  }
  return root_directory / "data";
};

Visualiser::Visualiser(const Coord& windowSize) : m_windowSize(windowSize), m_center({10, 10}) {
  auto params = TCOD_ContextParams{};
  params.tcod_version = TCOD_COMPILEDVERSION;
  // params.argc = 0;
  // params.argv = {""};
  params.renderer_type = TCOD_RENDERER_SDL2;
  params.vsync = 1;
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.window_title = "Libtcod Template Project";

  auto tileset = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
  params.tileset = tileset.get();

  m_console = tcod::Console{80, 40};
  params.console = m_console.get();

  m_context = tcod::Context(params);
}

Visualiser::~Visualiser() {}

void Visualiser::setMap(std::unique_ptr<Map> map) { m_map = std::move(map); }

void Visualiser::showMap() const {
  m_console.clear();
  tcod::print(m_console, {0, 0}, "Hello World", TCOD_white, std::nullopt);

  auto startPos = m_center;  // getLeftUpCd(m_center);
  auto endPos = m_center + m_windowSize;

  for (auto x = startPos.x; x < endPos.x; ++x) {
    for (auto y = startPos.y; y < endPos.y; ++y) {
      auto id = m_map->getIdentifier({x, y});
      showId({x, y}, id);
    }
  }

  showBorder();

  m_context.present(m_console);
}

void Visualiser::showId(std::array<int, 2>&& cd, const Identifier& id) const {
  static std::string s = " ";
  s[0] = id;
  tcod::print(m_console, cd, s, TCOD_white, std::nullopt);
}

// void Visualiser::setConsole(tcod::Console& console) { m_console = console; }
Coord Visualiser::getLeftUpCd(const Coord& center) const { return center - m_windowSize.del(2); }
void Visualiser::showBorder() const {
  // m_center;
  // auto x = m_center.x - 1;
  for (auto x : {m_center.x - 1, m_center.x + m_windowSize.x}) {
    for (auto y = m_center.y - 1; y < m_center.y + m_windowSize.y + 1; ++y) {
      showId({x, y}, BORDER_VERT);
    }
  }

  for (auto y : {m_center.y - 1, m_center.y + m_windowSize.y}) {
    for (auto x = m_center.x; x < m_center.x + m_windowSize.x; ++x) {
      showId({x, y}, BORDER_HOR);
    }
  }
}
