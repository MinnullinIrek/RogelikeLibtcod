#include "map_generator.h"

#include <assert.h>
#include <stdio.h>

#include <algorithm>
#include <list>
#include <map>

#include "constants.h"
#include "maps/map.h"
#include "units_factory.h"
#include "utils/random.h"
#include "unit_types.h"
MapGenerator::MapGenerator(std::weak_ptr<UnitsFactory> unitFactory) : m_unitFactory(unitFactory) {}

MapGenerator::~MapGenerator() {}
/* void MapGenerator::generateRoom(const int& roomsCount, const Coord& size) {
 

  for (auto i = roomsCount; i > 0; --i) {
    auto unitFactory = m_unitFactory.lock();
    auto cord = RoomCoord{
        {random<int>(0, size.x - 1), random<int>(0, size.y - 1)},
        {random<int>(0, size.x - 1), random<int>(0, size.y - 1)}};
    //auto coord_s = Coord{random<int>(0, size.x - 1), random<int>(0, size.y - 1)};
    //auto coord_e = Coord{random<int>(0, size.x - 1), random<int>(0, size.y - 1)};
    //while (std::find(std::begin(randomRoomCord), std::end(randomRoomCord), coord_s) != std::end(randomRoomCord)) {

    //}
    //while (std::find(std::begin(randomRoomCord), std::end(randomRoomCord), coord_e) != std::end(randomRoomCord)) {

    //}
    randomRoomCord[i] = cord, unitFactory->createWall();
  }

}*/
std::shared_ptr<Map> MapGenerator::generateRandomMap(const Coord& size) {
  auto map = std::make_shared<Map>(size);
  const auto wallsCount = static_cast<int>(size.x * size.y * RROOM_PERCENT);
  auto unitFactory = m_unitFactory.lock();
  assert(unitFactory);
  LineCord* lineCord = new LineCord[wallsCount];
  for (int i = 1; i < wallsCount;i++) {
    if (i % 2 == 0) {
      int randomNumY = random(0, size.y - 1);
      lineCord[i].cord_s = {0, randomNumY};
      lineCord[i].cord_e = {size.x - 1, randomNumY};
    } else {
      int randomNumX = random(0,size.x - 1);
      lineCord[i].cord_s = {randomNumX,0};
      lineCord[i].cord_e = {randomNumX, size.y - 1};
    }
    }
  for (int i = 0; i < wallsCount; i++) {
    if (lineCord[i].cord_s.x < lineCord[i].cord_e.x) {
      for (int j = 0; j < i; j++) {
        if (map->getCell({j ,lineCord[i].cord_s.y }) == map->getCell({ j - 1, lineCord[i].cord_s.y})) {
          // map->setUnit(unitFactory->createNull(), {lineCord[i].cord_s.x, j / 2});
          map->setUnit(nullptr, {j / 2, lineCord[i].cord_s.y});
        }
        map->setUnit(unitFactory->createWall(), {j, lineCord[i].cord_s.y});
      }
    }
    if (lineCord[i].cord_s.y < lineCord[i].cord_e.y) {
      for (int j = 0; j < i; j++) {
        if (map->getCell({lineCord[i].cord_s.x, j}) == map->getCell({lineCord[i].cord_s.x, j - 1})) {
          //map->setUnit(unitFactory->createNull(), {lineCord[i].cord_s.x, j / 2});
          map->setUnit(nullptr, {lineCord[i].cord_s.x, j / 2});
          
        }
        map->setUnit(unitFactory->createWall(), {lineCord[i].cord_s.x, j});
      }
    }    
  }

  //for (int i = 0; i < wallsCount; i++) {
    delete[] lineCord;
  //}
  /*
  std::list<Coord> existingCoords;
  for (auto i = wallsCount; i > 0; --i) {
    auto coord = Coord{random<int>(0, size.x - 1), random<int>(0, size.y - 1)};

    while (std::find(std::begin(existingCoords), std::end(existingCoords), coord) != std::end(existingCoords)) {
      coord = Coord{random<int>(0, size.x - 1), random<int>(0, size.y - 1)};
    }
    existingCoords.push_back(coord);
    map->setUnit(unitFactory->createWall(), coord);
  }*/
  
  return map;
}
