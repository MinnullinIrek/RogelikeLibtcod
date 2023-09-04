#include "map_generator.h"

#include <assert.h>
#include <stdio.h>

#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include <array>
#include <thread>
#include <chrono>

#include "constants.h"
#include "maps/map.h"
#include "units_factory.h"
#include "utils/random.h"
#include "unit_types.h"
#include "maps/room.h"
#include "visualiser/visualiser.h"
#include "logger.h"
#include <cmath>

using namespace std::chrono_literals;

std::list<RoomStart> RoomStart::deliver(bool isHorizontal) const {
  static const int minRoomSize = 5;

  RoomStart r1(m_borders);
  RoomStart r2(m_borders);

  auto& param1 = isHorizontal ? r1.m_borders[1].x : r1.m_borders[1].y;
  auto& param2 = isHorizontal ? r2.m_borders[0].x : r2.m_borders[0].y; 

  if (param1 - param2 <= 2 * minRoomSize) {
    return {m_borders};
  }

  const auto& border0 = isHorizontal ? m_borders[0].x : m_borders[0].y;
  const auto& border1 = isHorizontal ? m_borders[1].x : m_borders[1].y;

  
  param1 = random<int>(border0 + minRoomSize, border1 - minRoomSize);
  param2 = param1+1;

  return {r1, r2};
}

std::list<Coord> RoomStart::getAllCoords() {
  std::list<Coord> coords;

  const auto& leftUpCd = m_borders[0];
  const auto& rightDownCd = m_borders[1];

  for (int x = leftUpCd.x; x <= rightDownCd.x; ++x) {
    for (int y = leftUpCd.y; y <= rightDownCd.y; ++y) {
      coords.push_back({x, y});
    }
  }

  return coords;
}


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

  
std::list<RoomStart> MapGenerator::delivereMap(const Coord& size) {

  std::list<RoomStart> rooms;

  rooms.push_back(RoomStart({Coord(0, 0), Coord(size.x - 1, size.y - 1)}));
  const int roomCellCount = 10;
  const int delivereCount = log2(size.x * size.y / roomCellCount);
  //rooms.reserve(roomsCount);
  bool isHorizontal = true;

  
  LOG("start delivereCount = %d \n",  delivereCount);

  for (auto i = 0; i < delivereCount; ++i) {

    LOG("   for curI = [%d] roomsCount = [%d] \n", i, rooms.size());

    std::list<RoomStart> newRooms;
    int roomCnt = -1;
    for (auto room : rooms) {
      auto genartedRooms =  room.deliver(isHorizontal);
      LOG("      roomCnt = [%d] \n", ++roomCnt);

      for (const auto& r : genartedRooms) {
        newRooms.push_back(r);
      }
    }
    isHorizontal = !isHorizontal; 

    rooms = newRooms;
    //отрисовка для наглядности
    m_visualizer->clear();
    for (auto room : rooms) {
      auto coords =  room.getAllCoords();
      
      int r = random<int>(0, 255);
      int g = random<int>(0, 255);
      int b = random<int>(0, 255);

      m_visualizer->showCoords(coords, r, g, b);
    }
    m_visualizer->show();
    std::this_thread::sleep_for(5000ms);
  }

  return rooms;
}


void MapGenerator::setVisualiser(std::shared_ptr<Visualiser> visualiser) { m_visualizer = visualiser;}
