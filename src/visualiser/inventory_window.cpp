#include "inventory_window.h"

#include <algorithm>
#include <iterator>

#include "../Bag.h"
#include "../units/BodyPart.h"
#include "../units/IUnit.h"
// #include "window.h"

InventoryWindow::InventoryWindow(const Rectangle& rect) : IWindow(rect) {}

void InventoryWindow::setHero(std::shared_ptr<Unit> hero) {
  m_hero = hero;
  if (hero) {
    // Подписываемся на героя: Unit и BodyParts каскадно шлют emit при экипировке.
    hero->addSubscriber(std::static_pointer_cast<Subscriber>(std::shared_ptr<Subscriber>(hero.get(), [](Subscriber*) {})));
  }
}

void InventoryWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  Text text{"", Color{255, 255, 255}, Color{0, 0, 0}};
  Coord cd = parentCd + Coord{0, 0};
  for (auto it = m_itemsLeft.begin(); it != m_itemsLeft.end(); ++it) {
    cd.y += 1;
    visualizator({it->m_text, Color{255, 255, 255}, Color{0, 0, 0}}, cd);
  }
  // Правая колонка — экипированные предметы (части тела).
  Coord rcd = parentCd + Coord{m_rectangle.width() / 2, 0};
  rcd.y += 1;
  visualizator({m_headerRight.m_text, Color{255, 255, 0}, Color{0, 0, 0}}, rcd);
  for (auto it = m_itemsRight.begin(); it != m_itemsRight.end(); ++it) {
    rcd.y += 1;
    visualizator({it->m_text, Color{255, 255, 255}, Color{0, 0, 0}}, rcd);
  }
}

void InventoryWindow::notify(std::weak_ptr<Publisher> pbag) {
  auto lockPbag = pbag.lock();
  auto count = m_rectangle.height();
  if (lockPbag) {
    auto bag = std::dynamic_pointer_cast<Bag>(lockPbag);
    if (bag) {
      auto bagItems = bag->showBag(count);
      m_itemsLeft.clear();
      std::copy(bagItems.begin(), bagItems.end(), std::back_inserter(m_itemsLeft));
    }
  }
  // Обновляем список надетых вещей по частям тела.
  auto hero = m_hero.lock();
  m_itemsRight.clear();
  m_headerRight = Text("Worn items:", Color{255, 255, 0}, Color{0, 0, 0});
  if (hero && hero->getBodyParts()) {
    auto equipped = hero->getBodyParts()->showEquipped(count - 1);
    int sel = hero->getEquippedSelected();
    int i = 0;
    for (auto& t : equipped) {
      if (i == sel) t.m_text = "> " + t.m_text;
      m_itemsRight.push_back(t);
      ++i;
    }
  }
}
