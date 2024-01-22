#ifndef VISUALISER_H
#define VISUALISER_H

#include <array>
#include <libtcod.hpp>
#include <list>
#include <memory>

#include "../header.h"
#include "visualiser_interface.h"

class Tab;
class IWindow;
class MapGenerator;

class Visualiser : public VisualiserInterface {
  friend MapGenerator;

 public:
  Visualiser(const Coord& windowSize);
  ~Visualiser();
  void show() const override;
  void addWindow(std::shared_ptr<IWindow> window) override;

 protected:
  void showId(std::array<int, 2>&& cd, const Identifier& id) const;
  TCOD_ColorRGB convertColor(const Color& color) const;

 private:
  void clear();
  void showBorder() const;
  void showCoords(std::list<Coord> coords, unsigned int r, unsigned int g, unsigned int b);

  void showBuffer();
  std::shared_ptr<Tab> createTab(std::string_view name);

 private:
  Coord m_windowSize;

  mutable tcod::Console m_console;
  mutable tcod::Context m_context;
  const Coord m_center;
  std::list<std::shared_ptr<Tab>> m_tabs;
  std::shared_ptr<Tab> m_currentTab;
  std::vector<std::shared_ptr<IWindow>> m_windows;
};

#endif
