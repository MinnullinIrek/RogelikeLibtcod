#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H
#include <memory>
#include "header.h"

class Map;
class UnitsFactory;

class MapGenerator {
 public:
  MapGenerator(std::weak_ptr<UnitsFactory> unitFactory);
  ~MapGenerator();
  std::shared_ptr<Map> generateRandomMap(const Coord& size);

 private:
  void generateRoom(const int& roomCount, const Coord& size);
  std::weak_ptr<UnitsFactory> m_unitFactory;
};

#endif
