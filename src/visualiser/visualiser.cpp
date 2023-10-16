#include "visualiser.h"

#include <SDL.h>
#include <assert.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "../info.h"
#include "../maps/map.h"
#include "iwindow.h"
#include "tab.h"

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

void Visualiser::setMap(std::shared_ptr<Map> map) {
  m_map = map;

  auto tab = createTab("map");
  // auto nameKey = tab->addWindow(std::make_shared<Window>("map"));

  // auto mapWindow = std::m
}

std::shared_ptr<Tab> Visualiser::createTab(std::string_view name) {
  static auto showLambda = [this](Text&& text, const Coord& cd) {
    tcod::print(m_console, {cd.x, cd.y}, text.m_text, convertColor(text.m_color), convertColor(text.m_bgColor));
  };
  auto tab = std::make_shared<Tab>(name, showLambda);

  return tab;
}

void Visualiser::showMap() const {
  m_console.clear();

  static auto showLamb = [this](Text&& text, const Coord& cd) {
    tcod::print(m_console, {cd.x, cd.y}, text.m_text, convertColor(text.m_color), convertColor(text.m_bgColor));
  };

  for (auto& win : m_windows) {
    win->show(showLamb, {0, 0});
  }

  // showBorder();
  // showInfo();
  m_context.present(m_console);
}

void Visualiser::showInfo() const {
  assert(m_info);
  auto text = m_info->getText();
  tcod::print(m_console, {40, 10}, text, TCOD_ColorRGB{255, 255, 255}, TCOD_ColorRGB{0, 0, 255});
}

void Visualiser::showId(std::array<int, 2>&& cd, const Identifier& id) const {
  static std::string s = " ";
  s[0] = id.symbol;
  // uint8_t r, g, b;
  tcod::print(
      m_console,
      cd,
      s,
      TCOD_ColorRGB{id.color.r, id.color.g, id.color.b},
      TCOD_ColorRGB{id.bgColor.r, id.bgColor.g, id.bgColor.b});
}

void Visualiser::showBorder() const {
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

void Visualiser::setInfo(std::shared_ptr<Info> info) { m_info = info; }

void Visualiser::showCoords(std::list<Coord> coords, unsigned int r, unsigned int g, unsigned int b) {
  static std::string ch = "a";
  for (auto cd : coords) {
    tcod::print(
        m_console,
        {cd.x, cd.y},
        ch,
        TCOD_ColorRGB{static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)},
        std::nullopt);
  }
  ++ch[0];
  if (ch[0] > 'z') {
    ch[0] = 'a';
  }
}

void Visualiser::clear() { m_console.clear(); }
void Visualiser::show() { m_context.present(m_console); }

void Visualiser::showAgain() {
  static auto showLambda = [this](Text&& text, const Coord& cd) {
    tcod::print(m_console, {cd.x, cd.y}, text.m_text, convertColor(text.m_color), convertColor(text.m_bgColor));
  };

  if (m_currentTab) {
    m_currentTab->show(showLambda, {0, 0});
  }
}

TCOD_ColorRGB Visualiser::convertColor(const Color& color) const { return TCOD_ColorRGB{color.r, color.g, color.b}; }

void Visualiser::addWindow(std::shared_ptr<IWindow> window) { m_windows.push_back(window); }
