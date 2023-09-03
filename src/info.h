#ifndef INFO_H
#define INFO_H

#include <memory>
#include <string>
#include <unordered_map>

#include "header.h"

class Unit;

class Info {
 public:
  Info(/* args */);
  ~Info();
  void setHero(std::weak_ptr<Unit> hero);
  std::string getText();
  const Coord& getCoord() const;
  const std::unordered_map<Coord, bool, KeyHasher>& getWatchingCoords() const;

 private:
  std::weak_ptr<Unit> m_mainHero;
};

#endif
