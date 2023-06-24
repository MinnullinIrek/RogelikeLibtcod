#include "chars.h"

#include <algorithm>

Char::Char(CharType startValue) : m_value(startValue) {}
Char::Char(const Char& ch) : m_value(ch.getValue()), m_subscribers(ch.m_subscribers) {}
Char::Char(const Char&& ch) : m_value(ch.getValue()), m_subscribers(std::move(ch.m_subscribers)) {}

Char::~Char() {}

CharType Char::getValue() const { return m_value; }

Char::operator CharType() const { return getValue(); }

void Char::setValue(const CharType& ch) {
  m_value = ch;
  std::for_each(std::begin(m_subscribers), std::end(m_subscribers), [ch](auto& sub) { sub.second(ch); });
}

SubKey Char::addSubscriber(const std::function<void(CharType)>& sub) {
  ++m_lastKey;
  m_subscribers.emplace(m_lastKey, sub);
  return m_lastKey;
}

void Char::removeSubscriber(SubKey key) { m_subscribers.erase(key); }

bool operator==(const Char& lhs, const Char& rhs) { return lhs.getValue() == rhs.getValue(); }
bool operator!=(const Char& lhs, const Char& rhs) { return lhs.getValue() != rhs.getValue(); }
bool operator<(const Char& lhs, const Char& rhs) { return lhs.getValue() < rhs.getValue(); }
Char operator+(const Char& lhs, const Char& rhs) { return Char(lhs.getValue() + rhs.getValue()); }
Char operator-(const Char& lhs, const Char& rhs) { return Char(lhs.getValue() + rhs.getValue()); }
Char& Char::operator+=(const Char& rhs) {
  setValue(getValue() + rhs.getValue());
  return *this;
}
Char& Char::operator-=(const Char& rhs) {
  setValue(getValue() + rhs.getValue());
  return *this;
}

Chars::Chars() {}
Chars::~Chars() {}
void Chars::setValue(ECharTypes chType, CharType value) {
  if (m_chars.find(static_cast<int>(chType)) != std::end(m_chars)) {
    m_chars.at(static_cast<int>(chType)).setValue(value);
  } else {
    auto ch = Char(value);
    m_chars.insert(std::make_pair(static_cast<int>(chType), ch));
  }
}

CharType Chars::getValue(ECharTypes chType) { return m_chars.at(static_cast<int>(chType)).getValue(); }
SubKey Chars::addSubscriber(ECharTypes chType, const std::function<void(CharType)>& sub) {
  return m_chars.at(static_cast<int>(chType)).addSubscriber(sub);
}
void Chars::removeSubscriber(ECharTypes chType, SubKey key) {
  m_chars.at(static_cast<int>(chType)).removeSubscriber(key);
}
