#include "header.h"

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

std::string Coord::toString() const { return std::string("[") + std::to_string(x) + "," + std::to_string(y) + "]"; }
Distance Coord::length() const { return sqrt(x*x + y*y); }
