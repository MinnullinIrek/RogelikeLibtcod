#ifndef CHARS_H
#define CHARS_H
#include <functional>
#include <map>
#include <unordered_map>

#include "../char_types.h"
#include "../header.h"
#include "../utils/subscriber.h"

class Char : public Publisher {
 public:
  Char(CharType startValue);
  Char(const Char& ch);
  Char(const Char&& ch);

  ~Char();

  void setValue(const CharType& ch);
  CharType getValue() const;

  Char& operator-=(const Char& rhs);
  Char& operator+=(const Char& rhs);
  operator CharType() const;

 private:
  CharType m_value = 0;
};

class Chars final : public Publisher {
 public:
  Chars() = default;
  ~Chars() = default;
  void setValue(int chType, CharType value);
  CharType getValue(int chType);

 private:
  std::unordered_map<int, std::shared_ptr<Char>> m_chars;
};

#endif
