#include "inventory_window.h"

#include <algorithm>
#include <iterator>

#include "../Bag.h"
#include "../units/BodyPart.h"
#include "../units/IUnit.h"

InventoryWindow::InventoryWindow(const Rectangle& rect) : IWindow(rect) {
  m_headerLeft = Text("Bag:", Color{255, 255, 0}, Color{0, 0, 0});
  m_headerRight = Text("Body parts:", Color{255, 255, 0}, Color{0, 0, 0});
}

void InventoryWindow::setHero(std::shared_ptr<Unit> hero) {
  m_hero = hero;
  if (hero) {
    // Подписываемся на героя: Unit и BodyParts каскадно шлют emit при экипировке.
    hero->addSubscriber(hero);
    // Части тела тоже шлют emit напрямую — подписываемся и на них.
    if (auto bp = hero->getBodyParts()) {
      bp->addSubscriber(hero);
    }
    refresh();
  }
}

void InventoryWindow::refresh() {
  auto hero = m_hero.lock();
  if (!hero) return;

  // Левая колонка — сумка.
  auto count = m_rectangle.height();
  m_itemsLeft.clear();
  if (auto bag = hero->getBag()) {
    auto bagItems = bag->showBag(count - 1);
    std::copy(bagItems.begin(), bagItems.end(), std::back_inserter(m_itemsLeft));
  }

  // Правая колонка — ВСЕ части тела, даже пустые ("head: empty").
  m_itemsRight.clear();
  if (auto bp = hero->getBodyParts()) {
    int highlight = hero->getHighlightedSlotIndex();
    auto slots = bp->showSlots(highlight);
    std::copy(slots.begin(), slots.end(), std::back_inserter(m_itemsRight));
  }
}

void InventoryWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  // Левая колонка — сумка.
  Coord cd = parentCd + Coord{0, 0};
  visualizator({m_headerLeft.m_text, m_headerLeft.m_color, m_headerLeft.m_bgColor}, cd);
  for (auto it = m_itemsLeft.begin(); it != m_itemsLeft.end(); ++it) {
    cd.y += 1;
    visualizator({it->m_text, it->m_color, it->m_bgColor}, cd);
  }
  // Правая колонка — части тела и что надето (пустые слоты тоже видны).
  Coord rcd = parentCd + Coord{m_rectangle.width() / 2, 0};
  visualizator({m_headerRight.m_text, m_headerRight.m_color, m_headerRight.m_bgColor}, rcd);
  for (auto it = m_itemsRight.begin(); it != m_itemsRight.end(); ++it) {
    rcd.y += 1;
    visualizator({it->m_text, it->m_color, it->m_bgColor}, rcd);
  }
}

void InventoryWindow::notify(std::weak_ptr<Publisher> pbag) {
  auto lockPbag = pbag.lock();
  if (lockPbag) {
    auto bag = std::dynamic_pointer_cast<Bag>(lockPbag);
    if (bag) {
      auto count = m_rectangle.height();
      auto bagItems = bag->showBag(count - 1);
      m_itemsLeft.clear();
      std::copy(bagItems.begin(), bagItems.end(), std::back_inserter(m_itemsLeft));
    }
  }
  // Части тела всегда пересчитываются от героя — в т.ч. после equip/unequip.
  auto hero = m_hero.lock();
  m_itemsRight.clear();
  if (hero && hero->getBodyParts()) {
    int highlight = hero->getHighlightedSlotIndex();
    auto slots = hero->getBodyParts()->showSlots(highlight);
    std::copy(slots.begin(), slots.end(), std::back_inserter(m_itemsRight));
  }
}
