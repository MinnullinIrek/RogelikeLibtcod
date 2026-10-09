#include "info.h"

#include "char_names.h"
#include "char_types.h"
#include "units/IUnit.h"
#include "units/chars.h"
#include "units/mover.h"

Info::Info(/* args */) {}

Info::~Info() {}

void Info::setHero(std::weak_ptr<Unit> hero) { m_mainHero = hero; }

std::string Info::getText() {
  std::string text = "Info:\n___________\n";
  auto hero = m_mainHero.lock();
  if (hero) {
    auto& pos = hero->getMover()->getCoord();
    text += "pos: " + pos.toString() + "\n";
    auto chars = hero->getChars();
    if (chars) {
      for (int i = 0; i < static_cast<int>(ECharTypes::count); ++i) {
        text += charNames[i] + ": " + std::to_string(chars->getValue(i)) + "\n";
      }
    }
  }
  return text;
}

const Coord& Info::getCoord() const {
  auto hero = m_mainHero.lock();
  if (!hero) {
    throw std::string("Info::getCoord: no hero");
  }
  return hero->getMover()->getCoord();
}


const std::unordered_map<Coord, bool, KeyHasher>& Info::getWatchingCoords() const {
  auto hero = m_mainHero.lock();
  if (!hero) {
    throw("Info::getWatchingCoords() no hero");
  }
  return hero->getWatchingCoords();
}
