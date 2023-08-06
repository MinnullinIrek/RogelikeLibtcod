#include "info.h"

#include "units/IUnit.h"
#include "units/mover.h"

Info::Info(/* args */) {}

Info::~Info() {}

void Info::setHero(std::weak_ptr<Unit> hero) { m_mainHero = hero; }

std::string Info::getText() {
  std::string text;
  auto hero = m_mainHero.lock();
  if (hero) {
    auto pos = hero->getMover()->getCoord();
    text += "pos: " + pos.toString();
  }
  return text;
}
