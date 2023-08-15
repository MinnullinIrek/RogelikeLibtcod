#ifndef ROOM_H
#define ROOM_H

#include "../units_factory.h"
#include "../unit_types.h"
#include "../header.h"
#include "map.h"

#include <memory>
#include <map>

class Room{
 public:
  Room(std::shared_ptr<Map> map);
  std::map<Coord, std::shared_ptr<UnitsFactory>> randRoomChange();
  std::map<Coord, std::shared_ptr<UnitsFactory>> roomFinalChange();
  std::map<Coord, std::shared_ptr<UnitsFactory>> get_room() { return room; }
  bool changeEndCheck() { return changeEnd; }
 private:
  std::map<Coord, std::shared_ptr<UnitsFactory>> room;
  std::shared_ptr<Coord> m_size;
  std::shared_ptr<UnitsFactory> unitFactory;
  Coord startPos;
  Coord upLeftPos;
  std::shared_ptr<Map> m_map;
  bool changeEnd = false;
};
#endif
