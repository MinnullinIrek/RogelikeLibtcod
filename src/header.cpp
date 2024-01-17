#include "header.h"

#include "utils/random.h"

bool operator<(const Coord& c1, const Coord& c2) {
  if (c1.x == c2.x) {
    return c1.y > c2.y;
  }

  return c1.x < c2.x;
}

bool operator==(const Coord& c1, const Coord& c2) { return c1.x == c2.x && c1.y == c2.y; }

Coord operator+(const Coord& left, const Coord& right) { return Coord(left.x + right.x, left.y + right.y); }
Coord operator-(const Coord& left, const Coord& right) { return Coord(left.x - right.x, left.y - right.y); }

Coord Coord::del(float d) const { return Coord(static_cast<int>(x / d), static_cast<int>(y / d)); }
Coord Coord::operator/(float d) { return del(d); }

std::string Coord::toString() const { return std::string("[") + std::to_string(x) + "," + std::to_string(y) + "]"; }
Distance Coord::length() const { return static_cast<Distance>(sqrt(x * x + y * y)); }

Coord Coord::revert() const { return {-x, -y}; }

std::string timeToString(TimeType /*timeValue*/) { return "100 : 10:00"; }
