#include "map_generator.h"

#include <assert.h>
#include <stdio.h>

#include <algorithm>
#include <list>

#include "constants.h"
#include "maps/map.h"
#include "units_factory.h"
#include "utils/random.h"

MapGenerator::MapGenerator(std::weak_ptr<UnitsFactory> unitFactory) : m_unitFactory(unitFactory) {}

MapGenerator::~MapGenerator() {}

std::shared_ptr<Map> MapGenerator::generateRandomMap(const Coord& size) {
  auto map = std::make_shared<Map>(size);
  const auto wallsCount = static_cast<int>(size.x * size.y * RWALL_PERCENT);
  auto unitFactory = m_unitFactory.lock();
  assert(unitFactory);

  std::list<Coord> existingCoords;

  for (auto i = wallsCount; i > 0; --i) {
    auto coord = Coord{random<int>(0, size.x - 1), random<int>(0, size.y - 1)};

    while (std::find(std::begin(existingCoords), std::end(existingCoords), coord) != std::end(existingCoords)) {
      coord = Coord{random<int>(0, size.x), random<int>(0, size.y)};
    }
    existingCoords.push_back(coord);
    map->setUnit(unitFactory->createWall(), coord);
  }
  return map;
}
