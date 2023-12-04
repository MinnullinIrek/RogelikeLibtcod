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
#include "units/IUnit.h"
#include "units_factory.h"
#include "utils/random.h"
#include "visualiser/visualiser.h"

using namespace std::chrono_literals;

MapGenerator::MapGenerator(std::weak_ptr<UnitsFactory> unitFactory) : m_unitFactory(unitFactory) {}

MapGenerator::~MapGenerator() {}

std::shared_ptr<Map> MapGenerator::generateRandomMap(const Coord& size) {
  // const auto wallsCount = static_cast<int>(size.x * size.y * RROOM_PERCENT);
  auto unitFactory = m_unitFactory.lock();
  assert(unitFactory);

  // const int roomCellCount = 10;

  // const int delivereCount = size.x * size.y / roomCellCount;

  auto rooms = delivereMap(size);

  auto map = std::make_shared<Map>(size);

  for (int x = 0; x < size.x; ++x) {
    for (int y = 0; y < size.y; ++y) {
      map->setUnit(std::shared_ptr<IUnit>(new Unit('#', nullptr, EUnitTypes::wall)), Coord(x, y));
    }
  }

  for (const auto& room : rooms) {
    auto coords = room.getAllCoords(false);
    for (const auto& cd : coords) {
      map->setUnit(nullptr, cd);
    }
  }

  // unitFactory->createEnemy(); //todo create enemy

  return map;
}

std::list<RoomStart> MapGenerator::delivereMap(const Coord& size) {
  std::list<RoomStart> rooms;

  rooms.push_back(RoomStart({Coord(0, 0), Coord(size.x - 1, size.y - 1)}));
  const int roomCellCount = 10;
  const int delivereCount = static_cast<const int>(log2(size.x * size.y / roomCellCount));
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

      auto r = random<unsigned int>(0, 255);
      auto g = random<unsigned int>(0, 255);
      auto b = random<unsigned int>(0, 255);

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

    auto r = random<unsigned int>(0, 255);
    auto g = random<unsigned int>(0, 255);
    auto b = random<unsigned int>(0, 255);

    m_visualizer->showCoords(coords, r, g, b);
  }
  m_visualizer->show();
  // std::this_thread::sleep_for(5000ms);

  findNeighbors(rooms);

  makeCorridor(rooms);
  m_visualizer->clear();
  for (auto room : rooms) {
    auto coords = room.getAllCoords(false);

    auto r = random<unsigned int>(0, 255);
    auto g = random<unsigned int>(0, 255);
    auto b = random<unsigned int>(0, 255);

    m_visualizer->showCoords(coords, r, g, b);
  }
  m_visualizer->show();
  std::this_thread::sleep_for(1000ms);

  return rooms;
}

void MapGenerator::setVisualiser(std::shared_ptr<Visualiser> visualiser) { m_visualizer = visualiser; }

void MapGenerator::findNeighbors(std::list<RoomStart>& rooms) {
  for (auto it = rooms.begin(); it != rooms.end(); ++it) {
    auto j = it;
    ++j;
    auto r = random<unsigned int>(0, 255);
    auto g = random<unsigned int>(0, 255);
    auto b = random<unsigned int>(0, 255);

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

void MapGenerator::makeCorridor(std::list<RoomStart>& rooms) {
  rooms;

  auto start = rooms.begin();
  auto last = rooms.end();
  --last;
  auto it = start;
  for (;; ++it) {
    for (auto neighbor = it->m_neighbors.begin(); neighbor != it->m_neighbors.end(); ++neighbor) {
      auto corridorCoords = findWays(*it, **neighbor);
      // assert(!corridorCoords.empty());
      if (corridorCoords.empty()) {
        continue;
      }

      auto selectedCorrs = random<int>(0, static_cast<int>(corridorCoords.size() - 1));
      auto coords = corridorCoords.at(selectedCorrs);
      auto roomCor = RoomStart(std::array<Coord, 2>{coords.first, coords.second});
      roomCor.m_innerBorders = roomCor.m_borders;
      rooms.push_back(roomCor);
    }

    if (it == last) {
      break;
    }
  }
}

std::vector<std::pair<Coord, Coord>> MapGenerator::findWays(const RoomStart& room1, const RoomStart& room2) {
  std::vector<std::pair<Coord, Coord>> corridors;
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
