#include "map_window.h"

#include <cstdlib>

#include "../maps/cell.h"
#include "../maps/map.h"
#include "../units/IUnit.h"
#include "../units/mover.h"

MapWindow::MapWindow(const Rectangle& r) : IWindow(r) {}

void MapWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  for (const auto& cdCell : m_cells) {
    visualizator(cdCell.second, cdCell.first + parentCd);
  }
}

void MapWindow::notify(std::weak_ptr<Publisher> publisher) {
  auto locked = publisher.lock();
  if (locked) {
    auto mover = std::dynamic_pointer_cast<IMover>(locked);
    if (mover) {
      auto map = mover->getMap().lock();
      if (map) {
        //
        auto heroCoord = mover->getCoord();

        auto cell = map->getCell(heroCoord);
        auto h = cell->getUnit();
        auto hero = std::dynamic_pointer_cast<Unit>(h);
        //auto& w = hero->getWatchingCoords();

        auto& watchingCoords = std::dynamic_pointer_cast<Unit>(map->getCell(heroCoord)->getUnit())->getWatchingCoords();
        Coord windowSize = {m_rectangle.rd.x - m_rectangle.lu.x, m_rectangle.rd.x - m_rectangle.lu.y};

        Coord mapStart{0, 0};
        mapStart.x = std::max((heroCoord.x - (windowSize.x) / 2), 0);
        mapStart.y = std::max((heroCoord.y - (windowSize.y) / 2), 0);

        auto startPos = m_rectangle.lu;
        auto endPos = m_rectangle.lu + mapStart + windowSize - Coord{startPos.x, startPos.y};

        m_cells.clear();
        for (auto x = mapStart.x; x < endPos.x; ++x) {
          for (auto y = mapStart.y; y < endPos.y; ++y) {
            Coord cd = {x, y};
            auto id = map->getIdentifier(cd);
            if (watchingCoords.find(cd) == watchingCoords.end()) {
              static Color gray = {125, 125, 125};
              id.color = gray;
            }
            m_cells[{x + startPos.x - mapStart.x, y + startPos.y - mapStart.y}] = id;
          }
        }
      } else {
        throw("MapWindow::notify no map");
      }
    } else {
      throw("MapWindow::notify publisher not mover");
    }
  } else {
    throw("MapWindow::notify empty publisher");
  }
}
