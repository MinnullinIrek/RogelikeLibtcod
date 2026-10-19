#include "inventory_window.h"

#include <algorithm>
#include <iterator>
#include <sstream>

#include "../Bag.h"
#include "../items/IItems.h"
#include "../units/BodyPart.h"
#include "../units/IUnit.h"

namespace {
// Ширина строки Text (в символах), чтобы не печатать за пределы колонки.
int textWidth(const std::string& s) { return static_cast<int>(s.size()); }

// Абсолютная X окна Info (Rectangle{{40, 10}, ...} в main.cpp).
// В режиме инвентаря Info скрыт, и его место занимает колонка частей тела.
constexpr int kInfoX = 40;
// Высота блока Info (pos + характеристики) — ниже него начинается описание предмета.
constexpr int kInfoBlockHeight = 13;
}  // namespace

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

void InventoryWindow::refreshBodyParts() {
  auto hero = m_hero.lock();
  m_itemsRight.clear();
  m_highlightedSlot = -1;
  if (!hero) return;
  if (auto bp = hero->getBodyParts()) {
    m_highlightedSlot = hero->getHighlightedSlotIndex();
    auto slots = bp->showSlots(m_highlightedSlot);
    std::copy(slots.begin(), slots.end(), std::back_inserter(m_itemsRight));
  }

  // Описание выбранного в сумке предмета — печатается под колонкой частей тела,
  // на месте скрытого в этом режиме блока Info.
  m_itemDescription = Text("", Color{255, 255, 255}, Color{0, 0, 0});
  if (auto bag = hero->getBag()) {
    if (auto item = bag->getSelected()) {
      std::string desc = item->toString();
      if (!item->getDescription().empty()) {
        desc += "\n" + item->getDescription();
      }
      m_itemDescription = Text(desc, Color{255, 255, 255}, Color{0, 0, 0});
    }
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

  // Правая колонка — части тела.
  refreshBodyParts();
}

void InventoryWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  // Части тела всегда актуальны (подсветка зависит от выбранного в сумке предмета).
  refreshBodyParts();

  const int halfWidth = m_rectangle.width() / 2;

  // Общая печать колонки: header + items, обрезая строки по ширине колонки.
  auto renderColumn = [&](const Coord& start, const Text& header, const std::list<Text>& items) {
    Coord cd = start;
    visualizator({header.m_text, header.m_color, header.m_bgColor}, cd);
    for (const auto& it : items) {
      cd.y += 1;
      std::string line = it.m_text;
      if (textWidth(line) > halfWidth - 1) {
        line = line.substr(0, halfWidth - 1);
      }
      visualizator({line, it.m_color, it.m_bgColor}, cd);
    }
  };

  // Левая колонка — сумка (начинается с левого края окна).
  renderColumn(parentCd + m_rectangle.lu, m_headerLeft, m_itemsLeft);

  // Правая колонка — части тела. Смещена влево на место Info:
  // раньше Info рисовался по абсолютной X=40, теперь BodyParts стартуют оттуда же.
  renderColumn(Coord{parentCd.x + kInfoX, parentCd.y}, m_headerRight, m_itemsRight);

  // Описание выбранного предмета — под блоком Info (его высота зарезервирована),
  // там же по X. Печатаем построчно, чтобы многострочный текст встал на свои места.
  if (!m_itemDescription.m_text.empty()) {
    Coord descCd{parentCd.x + kInfoX, parentCd.y + kInfoBlockHeight};
    std::istringstream stream(m_itemDescription.m_text);
    std::string line;
    while (std::getline(stream, line)) {
      if (textWidth(line) > halfWidth - 1) {
        line = line.substr(0, halfWidth - 1);
      }
      visualizator({line, m_itemDescription.m_color, m_itemDescription.m_bgColor}, descCd);
      descCd.y += 1;
    }
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
  refreshBodyParts();
}
