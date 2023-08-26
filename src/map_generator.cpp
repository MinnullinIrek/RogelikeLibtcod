#include "map_generator.h"

#include <assert.h>
#include <stdio.h>

#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include <array>
#include <thread>

#include "constants.h"
#include "maps/map.h"
#include "units_factory.h"
#include "utils/random.h"
#include "unit_types.h"
#include "maps/room.h"
#include "visualiser/visualiser.h"


MapGenerator::MapGenerator(std::weak_ptr<UnitsFactory> unitFactory) : m_unitFactory(unitFactory) {}

MapGenerator::~MapGenerator() {}

std::shared_ptr<Map> MapGenerator::generateRandomMap(const Coord& size) {
  auto map = std::make_shared<Map>(size);
  const auto wallsCount = static_cast<int>(size.x * size.y * RROOM_PERCENT);
  auto unitFactory = m_unitFactory.lock();
  assert(unitFactory);

  const int roomCellCount = 10;

  const int delivereCount = size.x * size.y / roomCellCount;

  //1 ��������� ����� ��������� ���
  auto rooms = delivereMap(size);




  return map;
  
}

std::vector<RoomStart> MapGenerator::delivereMap(const Coord& size) {
  std::vector<RoomStart> rooms;
  rooms.push_back(RoomStart{Coord(0, 0), Coord(size.x - 1, size.y - 1)});
  const int roomCellCount = 10;
  const int delivereCount = size.x * size.y / roomCellCount;
  //rooms.reserve(roomsCount);

  for (auto i = 0; i < delivereCount; ++i) {
    std::vector<RoomStart> newRooms;
    bool isHorizontal = true;
    for (auto room : rooms) {
      auto genartedRooms =  room.deliver(isHorizontal);

      newRooms.push_back(genartedRooms.at(0));
      newRooms.push_back(genartedRooms.at(1));

      isHorizontal = !isHorizontal; 

    }



    rooms = newRooms;
    //отрисовка для наглядности
    for (auto room : rooms) {
      auto coords =  room.getAllCoords();
      //generate rgb
      int r, g,  b;
      m_visualizer->showCoords(coords, r, g, b);
    }


    //std::thread::thread::sleep(1 sek);

  }
}
