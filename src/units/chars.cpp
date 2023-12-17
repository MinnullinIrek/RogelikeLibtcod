#include "chars.h"

#include <algorithm>

Char::Char(int charType, CharValueType startValue) : Publisher(), m_value(startValue), m_charType(charType) {}
Char::Char(const Char& ch) : Publisher(), m_value(ch.getValue()), m_charType(ch.m_charType) {}
Char::Char(Char&& ch) noexcept : Publisher(), m_value(ch.m_value), m_charType(ch.m_charType) {
  Publisher::m_subscribers = std::move(ch.m_subscribers);
}

// Char::~Char() {}

CharValueType Char::getValue() const { return m_value; }

Char::operator CharValueType() const { return getValue(); }

void Char::setValue(const CharValueType& ch) {
  m_value = ch;
  emit();
}

bool operator==(const Char& lhs, const Char& rhs) { return lhs.getValue() == rhs.getValue(); }
bool operator!=(const Char& lhs, const Char& rhs) { return lhs.getValue() != rhs.getValue(); }
bool operator<(const Char& lhs, const Char& rhs) { return lhs.getValue() < rhs.getValue(); }
Char operator+(const Char& lhs, const Char& rhs) { return Char(lhs.m_charType, lhs.getValue() + rhs.getValue()); }
Char operator-(const Char& lhs, const Char& rhs) { return Char(lhs.m_charType, lhs.getValue() + rhs.getValue()); }
Char& Char::operator+=(const Char& rhs) {
  setValue(getValue() + rhs.getValue());
  return *this;
}
Char& Char::operator-=(const Char& rhs) {
  setValue(getValue() + rhs.getValue());
  return *this;
}

void Chars::setValue(int chType, CharValueType value) {
  if (m_chars.find(static_cast<int>(chType)) != std::end(m_chars)) {
    m_chars.at(static_cast<int>(chType))->setValue(value);
  } else {
    m_chars.insert(std::make_pair(static_cast<int>(chType), std::make_shared<Char>(chType, value)));
  }
}

CharValueType Chars::getValue(int chType) { return m_chars.at(static_cast<int>(chType))->getValue(); }

std::shared_ptr<Char> Chars::getChar(int chType) { return m_chars.at(chType); }
