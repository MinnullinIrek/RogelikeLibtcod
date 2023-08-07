#include "visualiser.h"

#include <SDL.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <assert.h>

#include "../maps/map.h"
#include "../info.h"

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

void Visualiser::setMap(std::shared_ptr<Map> map) { m_map = map; }

void Visualiser::showMap() const {
  m_console.clear();
  //tcod::print(m_console, {0, 0}, "Hello World", TCOD_ColorRGB{255, 255, 255}, std::nullopt);

  auto heroCoord = m_info->getCoord();

  heroCoord;
  m_windowSize;
  auto mapSize = m_map->getSize();

  Coord mapStart{0, 0};

  mapStart.x = std::max((heroCoord.x - m_windowSize.x / 2), 0);
  mapStart.y = std::max((heroCoord.y - m_windowSize.y / 2), 0);

  auto startPos = m_center;
  auto endPos = m_center + mapStart+ m_windowSize - Coord{startPos.x, startPos.y};




  for (auto x = mapStart.x; x < endPos.x ; ++x) {
    for (auto y = mapStart.y; y < endPos.y; ++y) {
      auto id = m_map->getIdentifier({x, y});
      showId({x + startPos.x - mapStart.x, y + startPos.y - mapStart.y}, id);
    }
  }

  showBorder();
  showInfo();
  m_context.present(m_console);

  
}

void Visualiser::showInfo() const {
  assert(m_info);
  auto text = m_info->getText();
  tcod::print(m_console, {40, 10}, text, TCOD_ColorRGB{255, 255, 255}, std::nullopt);
}
  

void Visualiser::showId(std::array<int, 2>&& cd, const Identifier& id) const {
  static std::string s = " ";
  s[0] = id;
  tcod::print(m_console, cd, s, TCOD_ColorRGB{255, 255, 255}, std::nullopt);
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

void Visualiser::setInfo(std::shared_ptr<Info> info) {
  m_info = info;
}
