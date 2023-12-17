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
  Char(int charType, CharValueType startValue);
  Char(const Char& ch);
  Char(Char&& ch) noexcept;

  ~Char() = default;

  void setValue(const CharValueType& ch);
  CharValueType getValue() const;

  Char& operator-=(const Char& rhs);
  Char& operator+=(const Char& rhs);
  operator CharValueType() const;

 public:
  const int m_charType;

 private:
  CharValueType m_value = 0;
};

class Chars final : public Publisher {
 public:
  Chars() = default;
  ~Chars() = default;
  virtual void setValue(int chType, CharValueType value);
  CharValueType getValue(int chType);
  std::shared_ptr<Char> getChar(int chType);

 private:
  std::unordered_map<int, std::shared_ptr<Char>> m_chars;
};

#endif
