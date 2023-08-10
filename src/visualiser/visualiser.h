#ifndef VISUALISER_H
#define VISUALISER_H

#include <array>
#include <libtcod.hpp>
#include <memory>

#include "../header.h"

class Map;
class Info;

class Visualiser {
 public:
  Visualiser(const Coord& windowSize);
  ~Visualiser();
  void setMap(std::shared_ptr<Map> map);
  void setInfo(std::shared_ptr<Info> info);
  void showMap() const;
  void showInfo() const;
  //Coord getLeftUpCd(const Coord& center) const;
  // void setConsole(tcod::Console& console);
  void showBorder() const;

 protected:
  void showId(std::array<int, 2>&& cd, const Identifier& id) const;

 private:
  Coord m_windowSize;
  std::shared_ptr<Map> m_map;
  std::shared_ptr<Info> m_info;

  mutable tcod::Console m_console;
  mutable tcod::Context m_context;
  const Coord m_center;
};

#endif
