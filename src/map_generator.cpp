#include "map_generator.h"

#include <assert.h>
#include <stdio.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <list>
#include <map>
#include <thread>
#include <vector>

#include "constants.h"
#include "logger.h"
#include "maps/map.h"
#include "maps/room.h"
#include "unit_types.h"
#include "units_factory.h"
#include "utils/random.h"
#include "visualiser/visualiser.h"

using namespace std::chrono_literals;
const int minRoomSize = 6;

std::list<RoomStart> RoomStart::deliver(bool isHorizontal) const {
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
  param2 = param1 + 1;

  return {r1, r2};
}

void RoomStart::innerBorder() {
  auto innerBorder = m_borders;

  auto startx = m_borders[0].x + 1;
  auto endx = 2 + (m_borders[0].x + m_borders[1].x) / 2 - minRoomSize / 2;

 

  innerBorder[0].x =
      random<int>(m_borders[0].x + 1, 2 + (m_borders[0].x + m_borders[1].x) / 2 - minRoomSize / 2);

  auto starty = m_borders[0].y + 1;
  auto endy = 2 +   (m_borders[0].y + m_borders[1].y) / 2 - minRoomSize / 2;

  

  innerBorder[0].y =
      random<int>(m_borders[0].y + 1, 2 + (m_borders[0].y + m_borders[1].y) / 2 - minRoomSize / 2);

  auto startx2 = (m_borders[0].x + m_borders[1].x) / 2 + minRoomSize / 2 - 1;
  auto endx2 = m_borders[1].x - 1;

  
  innerBorder[1].x =
      random<int>( (m_borders[0].x + m_borders[1].x) / 2 + minRoomSize / 2 - 1, m_borders[1].x - 1);


  auto starty2 = (m_borders[0].y + m_borders[1].y) / 2 + minRoomSize / 2 - 1;
  auto endy2 = m_borders[1].y - 1;

  

  innerBorder[1].y =
      random<int>((m_borders[0].y + m_borders[1].y) / 2 + minRoomSize / 2 - 1, m_borders[1].y - 1);

  if (abs(innerBorder[1].x - innerBorder[0].x) < 3) {
    int i = 0;
  }
  // return innerBorder;
  m_borders = innerBorder;
}

std::list<Coord> RoomStart::getAllCoords() const{
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

  // 1 ��������� ����� ��������� ���
  auto rooms = delivereMap(size);

  return map;
}

std::list<RoomStart> MapGenerator::delivereMap(const Coord& size) {
  std::list<RoomStart> rooms;

  rooms.push_back(RoomStart({Coord(0, 0), Coord(size.x - 1, size.y - 1)}));
  const int roomCellCount = 10;
  const int delivereCount = log2(size.x * size.y / roomCellCount);
  // rooms.reserve(roomsCount);
  bool isHorizontal = true;

  LOG("start delivereCount = %d \n", delivereCount);

  for (auto i = 0; i < delivereCount; ++i) {
    LOG("   for curI = [%d] roomsCount = [%d] \n", i, rooms.size());

    std::list<RoomStart> newRooms;
    int roomCnt = -1;
    for (auto room : rooms) {
      auto genartedRooms = room.deliver(isHorizontal);
      LOG("      roomCnt = [%d] \n", ++roomCnt);

      for (const auto& r : genartedRooms) {
        newRooms.push_back(r);
      }
    }
    isHorizontal = !isHorizontal;

    rooms = newRooms;
    // отрисовка для наглядности
    m_visualizer->clear();
    for (const auto& room : rooms) {
      auto coords = room.getAllCoords();

      int r = random<int>(0, 255);
      int g = random<int>(0, 255);
      int b = random<int>(0, 255);

      m_visualizer->showCoords(coords, r, g, b);
    }
    m_visualizer->show();
    //std::this_thread::sleep_for(5000ms);
  }

  for (auto& room : rooms) {
    room.innerBorder();
  }

  m_visualizer->clear();
  for (auto room : rooms) {
    auto coords = room.getAllCoords();

    int r = random<int>(0, 255);
    int g = random<int>(0, 255);
    int b = random<int>(0, 255);

    m_visualizer->showCoords(coords, r, g, b);
  }
  m_visualizer->show();
  std::this_thread::sleep_for(50000ms);

  return rooms;
}

void MapGenerator::setVisualiser(std::shared_ptr<Visualiser> visualiser) { m_visualizer = visualiser; }
