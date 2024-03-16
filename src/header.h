#ifndef HEADER_H
#define HEADER_H

#include <map>
#include <memory>
#include <string>

#include "unit_types.h"
#include "units_factory.h"

enum class EWindowType { ascii = 1, pictures = 2 };
#define ISCONSOLE 1
#define ISVISUALISED 2
#define CURRENTBUILD 3
// int APP_TYPE = static_cast<int>(EWindowType::ascii) | static_cast<int>(EWindowType::pictures);

using Setting = float;
using Count = int;
using Description = std::string;
using Distance = float;
using TimeType = float;

#define translate
// using Identifier = char;
struct Color {
  uint8_t r, g, b;
};
struct Identifier {
  Identifier(int code = 0, const std::string& debugDescription = "") : code(code), debugDescription(debugDescription) {}
  int code = 0;
  std::string debugDescription;
  operator int() { return code; }
  bool isShadowed = false;
};

struct ConsoleIdentifier {
  ConsoleIdentifier(char symbol = 'N', const Color& color = {255, 255, 255}, const Color& bgColor = {0, 0, 0})
      : symbol(symbol), color(color), bgColor(bgColor){};

  //Text toText() const { return Text(symbol, color, bgColor); }
  Color color = {255, 255, 255};
  Color bgColor = {0, 0, 0};
  char symbol = 'N';
};

struct VisIdentifier {
  std::string path;
};

constexpr Count MAXMAPSIZE = 1000;
const Identifier emptyID(1);
const Identifier notSeen(2);
const Identifier BORDER_VERT(3);
const Identifier BORDER_HOR(4);
const Distance VISIBLE_CONST = 0.8f;
// bool retTrue(bool b = true) { return b; }

enum class EAction { none, left, right, up, down, inventory, map, undo, charAction, effect };

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
  Coord revert() const;
  int x;
  int y;
};

struct Rectangle {
  Coord lu;
  Coord rd;
  int width() const { return rd.x - lu.x; }
  int height() const { return rd.y - lu.y; }
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
 public:
  virtual Identifier toChar() const = 0;
};

class ToString {
 public:
  virtual Description toString() const = 0;
};

struct Text {
  Text() : m_text(""), m_color({255, 255, 255}), m_bgColor({0, 0, 0}) {}
  Text(std::string_view text, const Color& color, const Color& bgColor)
      : m_text(text), m_color(color), m_bgColor(bgColor){};
  Text(char symbol, const Color& color, const Color& bgColor)
      : m_text(std::to_string(symbol)), m_color(color), m_bgColor(bgColor){};

  // Text(const Identifier& id) : m_text(" "), m_color(id.color), m_bgColor(id.bgColor) { m_text[0] = id.symbol; }

  std::string m_text;
  Color m_color;
  Color m_bgColor;
};

std::string timeToString(TimeType /*timeValue*/);

#endif
