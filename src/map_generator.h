#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <memory>
#include "header.h"
#include <assert.h>
#include <list>
#include <array>

class Map;
class UnitsFactory;
class Visualiser;

struct RoomStart {
  RoomStart()  {}
  RoomStart(std::array<Coord, 2>&& borders) : m_borders(borders) {}
  RoomStart(const std::array<Coord, 2>& borders) : m_borders(borders) {}
  std::array<Coord, 2> m_borders;

  std::list<Coord> getAllCoords();

  std::list<RoomStart> deliver(bool isHorizontal) const;

};


class MapGenerator {
 public:
  MapGenerator(std::weak_ptr<UnitsFactory> unitFactory);
  ~MapGenerator();
  std::shared_ptr<Map> generateRandomMap(const Coord& size);
  void setVisualiser(std::shared_ptr<Visualiser> visualiser);
 private:
  std::list<RoomStart> delivereMap(const Coord& size);
  void generateRoom(const int& roomCount, const Coord& size);
  std::weak_ptr<UnitsFactory> m_unitFactory;
  std::shared_ptr<Visualiser> m_visualizer;

};

#endif
