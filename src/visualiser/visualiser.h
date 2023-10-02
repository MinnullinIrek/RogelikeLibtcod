#ifndef VISUALISER_H
#define VISUALISER_H

#include <array>
#include <libtcod.hpp>
#include <list>
#include <memory>

#include "../header.h"

class Map;
class Info;
class Tab;
class Window;

class Visualiser {
 public:
  Visualiser(const Coord& windowSize);
  ~Visualiser();
  void setMap(std::shared_ptr<Map> map);
  void setInfo(std::shared_ptr<Info> info);
  void showMap() const;
  void showInfo() const;
  // Coord getLeftUpCd(const Coord& center) const;
  //  void setConsole(tcod::Console& console);
  void showBorder() const;
  void showCoords(std::list<Coord> coords, unsigned int r, unsigned int g, unsigned int b);
  void clear();
  void show();

  // void addTab(std::shared_ptr<Tab> tab, std::string_view name);
  void showAgain();
  std::shared_ptr<Tab> createTab(std::string_view name);

 protected:
  void showId(std::array<int, 2>&& cd, const Identifier& id) const;
  TCOD_ColorRGB convertColor(const Color& color);

 private:
  Coord m_windowSize;
  std::shared_ptr<Map> m_map;
  std::shared_ptr<Info> m_info;

  mutable tcod::Console m_console;
  mutable tcod::Context m_context;
  const Coord m_center;
  std::list<std::shared_ptr<Tab>> m_tabs;
  std::shared_ptr<Tab> m_currentTab;
};

#endif
