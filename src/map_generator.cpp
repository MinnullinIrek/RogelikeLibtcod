#include "map_generator.h"

#include <assert.h>
#include <stdio.h>

#include <algorithm>
#include <list>
#include <map>
#include <vector>

#include "constants.h"
#include "maps/map.h"
#include "units_factory.h"
#include "utils/random.h"
#include "unit_types.h"
MapGenerator::MapGenerator(std::weak_ptr<UnitsFactory> unitFactory) : m_unitFactory(unitFactory) {}

MapGenerator::~MapGenerator() {}

std::shared_ptr<Map> MapGenerator::generateRandomMap(const Coord& size) {
  auto map = std::make_shared<Map>(size);
  const auto wallsCount = static_cast<int>(size.x * size.y * RROOM_PERCENT);
  auto unitFactory = m_unitFactory.lock();
  assert(unitFactory);
  std::vector<LineCord> lineCord;
  lineCord.reserve(wallsCount);

  for (int i = 0; i < wallsCount - 1;i++) {
    if (i % 2 == 0) {
      int randomNumY = random(0, size.y - 1);

      Coord cord_s = {0, randomNumY};
      Coord cord_e = {size.x - 1, randomNumY};

      lineCord.push_back({cord_s, cord_e});

    } else {
      int randomNumX = random(0,size.x - 1);
      Coord cord_s = {randomNumX, 0};
      Coord cord_e = {randomNumX, size.y - 1};
      lineCord.push_back({cord_s, cord_e});

    }
    }
  for (int i = 0; i < wallsCount - 1; i++) {
    if (lineCord[i].cord_s.x < lineCord[i].cord_e.x) {
      for (int j = 0; j < size.x - 1; j++) {
        if (map->getCell({j + 1 ,lineCord[i].cord_s.y }) == map->getCell({ j - 1, lineCord[i].cord_s.y})) {
          // map->setUnit(unitFactory->createNull(), {lineCord[i].cord_s.x, j / 2});
          if (j > 0) {
            map->setUnit(nullptr, {j - 1, lineCord[i].cord_s.y});
          }
          if (j > 2) {
            map->setUnit(nullptr, {j - 2, lineCord[i].cord_s.y});
            map->setUnit(nullptr, {j - 3, lineCord[i].cord_s.y});
          }
          }
        map->setUnit(unitFactory->createWall(), {j, lineCord[i].cord_s.y});
      }
    }
    if (lineCord[i].cord_s.y < lineCord[i].cord_e.y) {
      for (int j = 0; j < size.y; j++) {
        if (map->isWall({lineCord[i].cord_s.x, j + 1}) == map->isWall({lineCord[i].cord_s.x, j - 1})) {
          //map->setUnit(unitFactory->createNull(), {lineCord[i].cord_s.x, j / 2});
          if (j > 0) {
            map->setUnit(nullptr, {lineCord[i].cord_s.x, j - 1});
          }
        }
        map->setUnit(unitFactory->createWall(), {lineCord[i].cord_s.x, j});
      }
    }    
  }

  return map;
}
