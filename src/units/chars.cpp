#include "chars.h"

#include <algorithm>

Char::Char(CharType startValue) : Publisher(), m_value(startValue) {}
Char::Char(const Char& ch) : m_value(ch.getValue()) {}
Char::Char(const Char&& ch) : m_value(ch.getValue()) {}

Char::~Char() {}

CharType Char::getValue() const { return m_value; }

Char::operator CharType() const { return getValue(); }

void Char::setValue(const CharType& ch) {
  m_value = ch;
  emit();
}

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

void Chars::setValue(int chType, CharType value) {
  if (m_chars.find(static_cast<int>(chType)) != std::end(m_chars)) {
    m_chars.at(static_cast<int>(chType))->setValue(value);
  } else {
    m_chars.insert(std::make_pair(static_cast<int>(chType), std::make_shared<Char>(value)));
  }
}

CharType Chars::getValue(int chType) { return m_chars.at(static_cast<int>(chType))->getValue(); }
