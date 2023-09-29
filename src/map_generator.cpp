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

bool checkNear(int x1, int x2) { return abs(x1 - x2) <= 1; }

bool isNear(int x1, int x2, int y1, int y2) {
  auto middleX = (x1 + x2) / 2;
  auto halfX = abs(x2 - x1) / 2;

  auto middleY = (y1 + y2) / 2;
  auto halfY = abs(y2 - y1) / 2;

  auto middleDistance = middleX - middleY;

  if (middleDistance == 0) {
    return true;
  }

  auto i = middleDistance / abs(middleDistance);

  auto result = middleX - i * halfX - (middleY + i * halfY);
  auto j = result / abs(result);
  return i * j <= 0;
}

const int minRoomSize = 6;

Coord RoomStart::getCenter() { return (m_borders[0] + m_borders[1]) / 2; }

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
  m_innerBorders = m_borders;
  static const auto minRoomSize = 8;

  // auto startx = m_borders[0].x + 1;
  // auto endx = 2 + (m_borders[0].x + m_borders[1].x) / 2 - minRoomSize / 2;

  m_innerBorders[0].x = random<int>(m_borders[0].x + 1, 2 + (m_borders[0].x + m_borders[1].x) / 2 - minRoomSize / 2);

  // auto starty = m_borders[0].y + 1;
  // auto endy = 2 + (m_borders[0].y + m_borders[1].y) / 2 - minRoomSize / 2;

  m_innerBorders[0].y = random<int>(m_borders[0].y + 1, 2 + (m_borders[0].y + m_borders[1].y) / 2 - minRoomSize / 2);

  // auto startx2 = (m_borders[0].x + m_borders[1].x) / 2 + minRoomSize / 2 - 1;
  // auto endx2 = m_borders[1].x - 1;

  m_innerBorders[1].x = random<int>((m_borders[0].x + m_borders[1].x) / 2 + minRoomSize / 2 - 1, m_borders[1].x - 1);

  // auto starty2 = (m_borders[0].y + m_borders[1].y) / 2 + minRoomSize / 2 - 1;
  // auto endy2 = m_borders[1].y - 1;

  m_innerBorders[1].y = random<int>((m_borders[0].y + m_borders[1].y) / 2 + minRoomSize / 2 - 1, m_borders[1].y - 1);

  // if (abs(innerBorder[1].x - innerBorder[0].x) < 3) {
  // int i = 0;
  // }
  // m_borders = innerBorder;
}

std::list<Coord> RoomStart::getAllCoords(bool outer) const {
  std::list<Coord> coords;
  const auto& borders = outer ? m_borders : m_innerBorders;

  const auto& leftUpCd = borders[0];
  const auto& rightDownCd = borders[1];

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

  auto rooms = delivereMap(size);


  return map;
}

std::list<RoomStart> MapGenerator::delivereMap(const Coord& size) {
  std::list<RoomStart> rooms;

  rooms.push_back(RoomStart({Coord(0, 0), Coord(size.x - 1, size.y - 1)}));
  const int roomCellCount = 10;
  const int delivereCount = log2(size.x * size.y / roomCellCount);
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
    // std::this_thread::sleep_for(5000ms);
  }

  for (auto& room : rooms) {
    room.innerBorder();
  }

  m_visualizer->clear();
  for (auto room : rooms) {
    auto coords = room.getAllCoords(false);

    int r = random<int>(0, 255);
    int g = random<int>(0, 255);
    int b = random<int>(0, 255);

    m_visualizer->showCoords(coords, r, g, b);
  }
  m_visualizer->show();
  // std::this_thread::sleep_for(5000ms);

  findNeighbors(rooms);

  makeCorridor(rooms);
  m_visualizer->clear();
  for (auto room : rooms) {
    auto coords = room.getAllCoords(false);

    int r = random<int>(0, 255);
    int g = random<int>(0, 255);
    int b = random<int>(0, 255);

    m_visualizer->showCoords(coords, r, g, b);
  }
  m_visualizer->show();
  std::this_thread::sleep_for(5000ms);


  return rooms;
}

void MapGenerator::setVisualiser(std::shared_ptr<Visualiser> visualiser) { m_visualizer = visualiser; }

void MapGenerator::findNeighbors(std::list<RoomStart>& rooms) {
  for (auto it = rooms.begin(); it != rooms.end(); ++it) {
    auto j = it;
    ++j;
    int r = random<int>(0, 255);
    int g = random<int>(0, 255);
    int b = random<int>(0, 255);

    m_visualizer->showCoords(it->getAllCoords(false), r, g, b);
    m_visualizer->show();
    // std::this_thread::sleep_for(1000ms);

    for (; j != rooms.end(); ++j) {
      if (j->isNeighbor(*it)) {
        it->m_neighbors.push_back(&(*j));
        m_visualizer->showCoords(j->getAllCoords(false), r, g, b);
        m_visualizer->show();
        // std::this_thread::sleep_for(1000ms);
      }
    }
    // std::this_thread::sleep_for(2000ms);
  }
}

bool RoomStart::isNeighbor(const RoomStart& room) {
  bool neighbor = false;
  auto nextI = {-1, 1};

  for (const auto& i : nextI) {
    const auto j = i == 1 ? 1 : 0;
    if (m_borders[j].x + i == room.m_borders[1 - j].x) {
      auto result =
          isNear(m_innerBorders[0].y, m_innerBorders[1].y, room.m_innerBorders[0].y, room.m_innerBorders[1].y);
      if (result) {
        return true;
      }
    }
  }

  for (const auto& i : nextI) {
    const auto j = i == 1 ? 1 : 0;
    if (m_borders[j].y + i == room.m_borders[1 - j].y) {
      auto result = isNear(m_borders[0].x, m_borders[1].x, room.m_borders[0].x, room.m_borders[1].x);
      if (result) {
        return true;
      }
    }
  }

  return false;
}

void MapGenerator::makeCorridor(std::list<RoomStart>& rooms) {
  rooms;

  auto start = rooms.begin();
  auto last = rooms.end();
  --last;
  auto it = start;
  int i = 0;
  for (;; ++it) {
    for (auto neighbor = it->m_neighbors.begin(); neighbor != it->m_neighbors.end(); ++neighbor) {
      printf("%d %d \n", __LINE__, i);
      auto corridorCoords = findWays(*it, **neighbor);
      printf("%d %d \n", __LINE__, i);

      //assert(!corridorCoords.empty());
      if (corridorCoords.empty()) {
        continue;
      }
      printf("%d %d \n", __LINE__, i);

      auto selectedCorrs = random<int>(0, corridorCoords.size() - 1);
      printf("%d %d \n", __LINE__, i);

      auto coords = corridorCoords.at(selectedCorrs);
      printf("%d %d \n", __LINE__, i);
      auto roomCor = RoomStart(std::array<Coord, 2>{coords.first, coords.second});
      printf("%d %d \n", __LINE__, i);
      roomCor.m_innerBorders = roomCor.m_borders;
      printf("%d %d \n", __LINE__, i);
      rooms.push_back(roomCor);
      printf("%d %d \n", __LINE__, i);

      ++i;
    }

    if (it == last) {
      break;
    }
  }
}

std::vector<std::pair<Coord, Coord>> MapGenerator::findWays(
    const RoomStart& room1, const RoomStart& room2)
{
  std::vector<std::pair<Coord, Coord>> corridors;
  room1;
  room2;

  const auto xL1 = room1.m_innerBorders[0].x;
  const auto xR1 = room1.m_innerBorders[1].x;

  const auto yU1 = room1.m_innerBorders[0].y;
  const auto yD1 = room1.m_innerBorders[1].y;

  const auto xL2 = room2.m_innerBorders[0].x;
  const auto xR2 = room2.m_innerBorders[1].x;

  const auto yU2 = room2.m_innerBorders[0].y;
  const auto yD2 = room2.m_innerBorders[1].y;

  auto middleX1 = (xL1 + xR1) / 2;
  auto middleX2 = (xL2 + xR2) / 2;

  auto middleY1 = (yU1 + yD1) / 2;
  auto middleY2 = (yU2 + yD2) / 2;

  auto isHorizontal = abs(middleX1 - middleX2) - abs(middleY1 - middleY2) > 0;

  std::vector<int> commonVars;

  const auto& cm11 = isHorizontal ? yU1 : xL1;
  const auto& cm12 = isHorizontal ? yD1 : xR1;

  const auto& cm21 = isHorizontal ? yU2 : xL2;
  const auto& cm22 = isHorizontal ? yD2 : xR2;

  for (auto c = MAX(cm11, cm21); c <= MIN(cm12, cm22); ++c) {
    commonVars.push_back(c);
  }

  auto middleNonCommon1 = isHorizontal ? middleX1 : middleY1;
  auto middleNonCommon2 = isHorizontal ? middleX2 : middleY2;

  for (auto cv : commonVars) {
    Coord cd1;
    Coord cd2;

    if (isHorizontal) {
      cd1.x = middleNonCommon1;
      cd2.x = middleNonCommon2;
      cd1.y = cv;
      cd2.y = cv;
    } else {
      cd1.y = middleNonCommon1;
      cd2.y = middleNonCommon2;
      cd1.x = cv;
      cd2.x = cv;
    }
    corridors.push_back({cd1, cd2});
  }

  return corridors;
}
