#ifndef ROOM_START
#define ROOM_START

#include <array>
#include <list>

#include "../../src/header.h"

const int minRoomSize = 8;

bool checkNear(int x1, int x2);
bool isNear(int x1, int x2, int y1, int y2);

struct RoomStart {
  RoomStart() {}
  RoomStart(std::array<Coord, 2>&& borders) : m_borders(borders) {}
  RoomStart(const std::array<Coord, 2>& borders) : m_borders(borders) {}

  std::list<Coord> getAllCoords(bool outer = true) const;

  std::list<RoomStart> deliver(bool isHorizontal) const;

  void innerBorder();
  Coord getCenter();
  bool isNeighbor(const RoomStart& room);

  std::array<Coord, 2> m_borders;
  std::array<Coord, 2> m_innerBorders;

  std::list<RoomStart*> m_neighbors;
};

#endif
