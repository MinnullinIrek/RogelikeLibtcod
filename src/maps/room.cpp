#include "room.h"
#include "../utils/random.h"

#include <vector>

Room::Room(std::shared_ptr<Map> map) : m_map(map) {
  Coord sizeMap = map->getSize();
    Coord startPos1 = Coord{random(0, sizeMap.x - 2), random(0, sizeMap.y - 2)};
  upLeftPos = Coord{startPos1.x + 1, startPos1.y - 1};
  map->setUnit(unitFactory->createWall(), startPos1);
  std::vector vec = {
      Coord{startPos1},
      Coord{startPos1.x + 1, startPos1.y},
      Coord{startPos1.x - 1, startPos1.y},
      Coord{startPos1.x, startPos1.y + 1},
      Coord{startPos1.x, startPos1.y - 1},
      Coord{startPos1.x + 1, startPos1.y + 1},
      Coord{startPos1.x + 1, startPos1.y - 1},
      Coord{startPos1.x - 1, startPos1.y + 1},
      Coord{startPos1.x - 1, startPos1.y - 1}};
  
  for (int i = 0; i < 9; i++) {
    room[{vec[i]}]->createWall();
    Coord cd = vec[i];
    if (cd.x > 0 || cd.y > 0) {
      m_map->setUnit(unitFactory->createWall(), cd);
  }
    }
 // m_size->x = 3;
  //m_size->y = 3;
};
std::map<Coord, std::shared_ptr<UnitsFactory>> Room::randRoomChange() {
  int selector = random(1, 4);
  int count = 0;
  auto map_size = m_map->getSize();
  while (count <= 4) {
    if (selector == 1 && !m_map->isWall({upLeftPos.x - 1, upLeftPos.y}) && upLeftPos.x > 0) {  // up
      for (int i = 0; i < m_size->y; i++) {
        room[{upLeftPos.x - 1, upLeftPos.y + i}]->createWall();
        m_map->setUnit(unitFactory->createWall(), {upLeftPos.x - 1, upLeftPos.y + i});
      }
      upLeftPos.x--;
      m_size->x++;
      return room;
    }
    if (selector == 2 && !m_map->isWall({upLeftPos.x - m_size->x, upLeftPos.y}) &&  
      upLeftPos.x + m_size->x - 1 < map_size.x - 1) {  // down
      for (int i = 0; i < m_size->y;i++) {
        room[{upLeftPos.x + m_size->x,upLeftPos.y + i}]->createWall();
        m_map->setUnit(unitFactory->createWall(), {upLeftPos.x + m_size->x, upLeftPos.y + i});
      }
      m_size->x++;
      return room;
    }
    if (selector == 3 && !m_map->isWall({upLeftPos.x,upLeftPos.y + m_size->y}) && upLeftPos.y + m_size->y - 1 < map_size.y) {  // right
      for (int i = 0; i < m_size->x;i++) {
        room[{upLeftPos.x + i, upLeftPos.y + m_size->x - 1}]->createWall();
        m_map->setUnit(unitFactory->createWall(), {upLeftPos.x + i, upLeftPos.y + m_size->x - 1});
      }

      m_size->y++;
      return room;
    }
    if (selector == 4 && !m_map->isWall({upLeftPos.x,upLeftPos.y - 1}) && upLeftPos.y > 0) {
      for (int i = 0; i < m_size->x;i++) {
        room[{upLeftPos.x + i, upLeftPos.y - 1}]->createWall();
        m_map->setUnit(unitFactory->createWall(), {upLeftPos.x + i, upLeftPos.y - 1});
      }
      m_size->y++;
      upLeftPos.y--;
      return room;
    }
    count++;
    selector = count;
  }
  changeEnd = true;
  return room;
}

std::shared_ptr<Map> Room::roomFinalChange() {
  for (int i = 1; i < m_size->x - 1;i++) {
    for (int j = 1; j < m_size->y - 1;j++) {
      room[{i, j}] = nullptr;
      m_map->setUnit(nullptr, {i, j});
    }
  }
  auto map_size = m_map->getSize();
  int wallCounter = 0;

  if (upLeftPos.x > 0) {
    for (int i = 0; i < m_size->y; i++) {
      if (m_map->isWall({upLeftPos.x - 1, i})) {
        wallCounter++;
        if (wallCounter == 5 && upLeftPos.x <= 1 ||
          wallCounter == 5 && !m_map->isWall({upLeftPos.x - 2, i}) && upLeftPos.x > 1) {
          room[{upLeftPos.x,i}] = nullptr;
          room[{upLeftPos.x - 1,i}] = nullptr;
          m_map->setUnit(nullptr, {upLeftPos.x, i});
          m_map->setUnit(nullptr, {upLeftPos.x - 1, i});
          wallCounter = 0;
        }
      }
    }
    wallCounter = 0;
  }
  if (upLeftPos.x + m_size->x - 1 < map_size.x - 1) {
    for (int i = 0; i < m_size->y; i++) {
      if (m_map->isWall({upLeftPos.x + m_size->x, i})) {
        wallCounter++;
        if (wallCounter == 5 && m_size->x >= map_size.x - 2 ||
            wallCounter == 5 && !m_map->isWall({upLeftPos.x + m_size->x + 1, i}) && upLeftPos.x + m_size->x < map_size.x) {
          room[{upLeftPos.x + m_size->x - 1, i}] = nullptr;
          room[{upLeftPos.x + m_size->x, i}] = nullptr;
          wallCounter = 0;
        }
      }
    }
    wallCounter = 0;
  }
  if (upLeftPos.y + m_size->y - 1 < map_size.y - 1) {
    for (int i = 0; i < m_size->x;i++) {
      if (m_map->isWall({i, upLeftPos.y + m_size->y})) {
        wallCounter++;
        if (wallCounter == 5 && m_size->y >= map_size.y - 2 ||
            wallCounter == 5 && !m_map->isWall({i, upLeftPos.y + m_size->y + 1}) && upLeftPos.y + m_size->y < map_size.y) {
          room[{i, upLeftPos.y + m_size->y - 1}] = nullptr;
          room[{i, upLeftPos.y + m_size->y}] = nullptr;
        }
      }
    }
    wallCounter = 0;
  }
  if (upLeftPos.y > 0) {
    for (int i = 0; i < m_size->x; i++) {
      if (m_map->isWall({i, upLeftPos.y - 1})) {
        wallCounter++;
        if (wallCounter == 5 && upLeftPos.y <= 1 ||
            wallCounter == 5 && !m_map->isWall({i, upLeftPos.y - 2}) && upLeftPos.y > 1) {
          room[{i, upLeftPos.y}] = nullptr;
          room[{i, upLeftPos.y - 1}] = nullptr;
          m_map->setUnit(nullptr, {i, upLeftPos.y});
          m_map->setUnit(nullptr, {i, upLeftPos.y - 1});
          wallCounter = 0;
        }
      }
    }
    wallCounter = 0;
  }



  return m_map;
}
