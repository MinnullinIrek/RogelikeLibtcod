#ifndef CHARS_H
#define CHARS_H
#include <functional>
#include <map>
#include <unordered_map>

#include "../char_types.h"
#include "../header.h"

using SubKey = int;

class Char {
 public:
  Char(CharType startValue);
  Char(const Char& ch);
  Char(const Char&& ch);

  ~Char();

  SubKey addSubscriber(const std::function<void(CharType)>& sub);
  void removeSubscriber(SubKey key);
  void setValue(const CharType& ch);
  CharType getValue() const;

  Char& operator-=(const Char& rhs);
  Char& operator+=(const Char& rhs);
  operator CharType() const;

 private:
  CharType m_value = 0;
  std::unordered_map<SubKey, std::function<void(CharType)>> m_subscribers;
  SubKey m_lastKey = 0;
};

class Chars final {
 public:
  Chars();
  ~Chars();
  void setValue(int chType, CharType value);
  CharType getValue(int chType);
  SubKey addSubscriber(int chType, const std::function<void(CharType)>& sub);
  void removeSubscriber(int chType, SubKey key);

 private:
  std::unordered_map<int, Char> m_chars;
};

#endif
