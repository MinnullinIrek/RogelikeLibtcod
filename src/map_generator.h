#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H
#include <memory>
#include "header.h"
#include <assert.h>

class Map;
class UnitsFactory;
class Visualiser;

struct RoomStart {
  std::array<Coord, 2> m_borders;

  std::list<Coord> getAllCoords() {
    std::list<Coord> coords;
    assert(false);
    return coords;
  }

  std::array<RoomStart, 2> deliver(bool isHorizontal) {
    RoomStart r1;
    RoomStart r2;
    r1.m_borders = m_borders;
    r2.m_borders = m_borders;

    if (isHorizontal) {
      r1.m_borders[1].x = random<int>(m_borders[0].x, m_borders[1].x);
      r2.m_borders[0].x = r1.m_borders[1].x;

    } else {
      assert(false);
      // todo
    }
    return {r1, r2};
  }
};


class MapGenerator {
 public:
  MapGenerator(std::weak_ptr<UnitsFactory> unitFactory);
  ~MapGenerator();
  std::shared_ptr<Map> generateRandomMap(const Coord& size);
  void setVisualiser(std::shared_ptr<Visualiser> visualiser);
 private:
  std::vector<RoomStart> delivereMap(const Coord& size);
  void generateRoom(const int& roomCount, const Coord& size);
  std::weak_ptr<UnitsFactory> m_unitFactory;
  std::shared_ptr<Visualiser> m_visualizer;

};

#endif
