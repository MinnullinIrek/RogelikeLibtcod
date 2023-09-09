#include "header.h"

#include "utils/random.h"

/*class  Room:
Room::Room()
{
  UnitsFactory unitfactory;
  for (int i = 0; i < 3;i++) {
    for (int j = 0; j < 3; j++) {
      room[{i,j}]->createWall();
    }
    }
  m_size->x = 3;
    m_size->y = 3;
}
void Room::roomchange()
{
  int selector = random(1,4);
  if (selector == 1) { //up
      for (int j = 0;j < m_size->y ; j++) {
        room[{m_size->x, j}]->createWall();
      }





  }

}


*///class Room;
bool operator<(const Coord& c1, const Coord& c2) {
  if (c1.x == c2.x) {
    return c1.y > c2.y;
  }

  return c1.x < c2.x;
}

bool operator==(const Coord& c1, const Coord& c2) { return c1.x == c2.x && c1.y == c2.y; }

Coord operator+(const Coord& left, const Coord& right) { return Coord(left.x + right.x, left.y + right.y); }
Coord operator-(const Coord& left, const Coord& right) { return Coord(left.x - right.x, left.y - right.y); }
// Coord Coord::operator/(float del) { return Coord(x / del, y / del); }
Coord Coord::del(float d) const { return Coord(x / d, y / d); }
Coord Coord::operator/(float d) { return del(d); }

std::string Coord::toString() const { return std::string("[") + std::to_string(x) + "," + std::to_string(y) + "]"; }
