#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include <assert.h>

#include <array>
#include <list>
#include <map>
#include <memory>
#include <vector>

#include "header.h"
#include "utils/room_start.h"

class Map;
class UnitsFactory;
class VisualiserInterface;
class Visualiser;

class MapGenerator {
 public:
  MapGenerator(std::weak_ptr<UnitsFactory> unitFactory);
  ~MapGenerator();
  std::shared_ptr<Map> generateRandomMap(const Coord& size);
  void setVisualiser(std::shared_ptr<VisualiserInterface> visualiser);

 private:
  std::list<RoomStart> delivereMap(const Coord& size);
  void findNeighbors(std::list<RoomStart>& rooms);
  void generateRoom(const int& roomCount, const Coord& size);
  void makeCorridor(std::list<RoomStart>& rooms);
  std::vector<std::pair<Coord, Coord> > findWays(const RoomStart& r1, const RoomStart& r2);
  std::weak_ptr<UnitsFactory> m_unitFactory;
  std::shared_ptr<Visualiser> m_visualizer;
};

#endif
