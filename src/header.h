#ifndef HEADER_H
#define HEADER_H

#include <map>
#include <memory>
#include <string>

#include "unit_types.h"
#include "units_factory.h"


using Setting = float;
using Count = int;
using Identifier = char;
using Description = std::string;

constexpr Count MAXMAPSIZE = 1000;
constexpr Identifier emptyID = '~';
constexpr Identifier BORDER_VERT = '*';
constexpr Identifier BORDER_HOR = '*';

// bool retTrue(bool b = true) { return b; }

enum class EAction { none, left, right, up, down };

enum class ItemType {
  simple,

};

enum class UnitType {
  simple,
};

struct Coord {
  Coord() : x(-1), y(-1){};
  Coord(int _x, int _y) : x(_x), y(_y){};
  friend bool operator<(const Coord& c1, const Coord& c2);
  friend bool operator==(const Coord& c1, const Coord& c2);

  friend Coord operator+(const Coord& left, const Coord& right);
  friend Coord operator-(const Coord& left, const Coord& right);
  Coord operator/(float del);
  Coord del(float d) const;
  std::string toString() const;
  int x;
  int y;
};
//
struct LineCord {
  Coord cord_s;
  Coord cord_e;
};
/* class Room {
 public:
  Room();
  void roomchange();
  std::map<Coord, std::shared_ptr<UnitsFactory>> get_room() { return room; }
 private:
  std::map<Coord, std::shared_ptr<UnitsFactory>> room;
  std::shared_ptr<Coord> m_size;
};*/
//
struct KeyHasher {
  std::size_t operator()(const Coord& k) const {
    using std::hash;
    using std::size_t;
    // using int;
    return ((hash<int>()(k.x) ^ (hash<int>()(k.y) << 1)) >> 1);
  }
};

class ToChar {
  virtual Identifier toChar() const = 0;
};

class ToString {
  virtual Description toString() const = 0;
};

#endif
