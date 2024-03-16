#ifndef IDENTIFIER_CONTAINER_H
#define IDENTIFIER_CONTAINER_H

#include "../header.h"
#include "unordered_map"

template <typename T>
class IdentifierContainer {
 public:
  void empty() {}
  T& get(const Identifier& id) { return m_container[id]; }
  //T& get(const Identifier& id)  { return m_container.at(id); };
  void set(const Identifier& id, T&& t) { m_container[id] = std::forward<T>(t); }

  T& operator[](const Identifier& index) { return m_container[index]; }
  T& operator[](const int index) { return m_container[index]; }
 // const T& operator[](const int index) const { return m_container[index]; }

 private:
  std::unordered_map<int, T> m_container;
};

IdentifierContainer<ConsoleIdentifier> createIdContainer() {
  IdentifierContainer<ConsoleIdentifier> container{};
  container[1] = ConsoleIdentifier('.');
  container[2] = ConsoleIdentifier(' ');
  container[3] = ConsoleIdentifier('*');
  container[4] = ConsoleIdentifier('*');

  return container;
}

IdentifierContainer<VisIdentifier> createVisContainer() {
  auto container = IdentifierContainer<VisIdentifier>{};

  return container;
}

#endif
