#ifndef HEADER_H
#define HEADER_H

#include <map>
#include <memory>
#include <string>

#include "unit_types.h"
#include "units_factory.h"

using Setting = float;
using Count = int;
using Description = std::string;
using Distance = float;

// using Identifier = char;
struct Color {
  uint8_t r, g, b;
};
struct Identifier {
  Identifier() {}
  Identifier(char symbol) : symbol(symbol){};
  // Identifier(const std::initializer_list<uint8_t>& color, const std::initializer_list<uint8_t>& bgColor, char symbol)
  //     : color(color),
  //       bgColor(bgColor),
  //       symbol(symbol){

  //       };

  Color color = {255, 255, 255};
  Color bgColor = {0, 0, 0};
  char symbol = 'N';
};

constexpr Count MAXMAPSIZE = 1000;
const Identifier emptyID = '.';
const Identifier notSeen = ' ';
const Identifier BORDER_VERT = '*';
const Identifier BORDER_HOR = '*';
const Distance VISIBLE_CONST = 0.8f;
// bool retTrue(bool b = true) { return b; }

enum class EAction { none, left, right, up, down, inventory };

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
  Distance length() const;
  int x;
  int y;
};

struct Rectangle {
  Coord lu;
  Coord rd;
};
//
struct LineCord {
  Coord cord_s;
  Coord cord_e;
};

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

struct Text {
  Text(std::string_view text, const Color& color, const Color& bgColor)
      : m_text(text), m_color(color), m_bgColor(bgColor){};

  std::string m_text;
  Color m_color;
  Color m_bgColor;
};

#endif
