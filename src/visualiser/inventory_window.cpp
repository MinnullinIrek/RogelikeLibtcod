#include "inventory_window.h"

#include <algorithm>
#include <iterator>
#include <sstream>

#include "../Bag.h"
#include "../char_names.h"
#include "../char_types.h"
#include "../items/IItems.h"
#include "../units/BodyPart.h"
#include "../units/IUnit.h"
#include "window.h"

namespace {
// Ширина строки Text (в символах), чтобы не печатать за пределы колонки.
int textWidth(const std::string& s) { return static_cast<int>(s.size()); }

// Абсолютная X колонки частей тела — там же, где раньше стоял Info.
constexpr int kBodyPartsX = 40;
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

  // Описание выбранного в сумке предмета — его печатает окно Info (main.cpp),
  // опущенное ниже колонок сумки/частей тела. Готовим текст здесь и обновляем Info.
  // Текст формируется через тот же renderInfoText, что и внутренний Info-блок,
  // но без заголовка "Info:".
  std::string desc;
  if (auto bag = hero->getBag()) {
    if (auto item = bag->getSelected()) {
      desc = item->toString();
      if (!item->getDescription().empty()) {
        desc += "\n" + item->getDescription();
      }
    }
  }
  m_itemDescription = Text(desc, Color{255, 255, 255}, Color{0, 0, 0});
  if (auto info = m_infoWindow.lock()) {
    // В режиме инвентаря описание печатает внутренний Info-блок окна (renderItemInfo),
    // а внешний Info в это время скрыт через MainWindow::addHideWindow (см. main.cpp) —
    // иначе два Info перекрывали бы друг друга и описание казалось «пропавшим».
    info->setText(desc.empty() ? "Info:" : ("Info:\n" + desc));
  }
}

// Печать описания выбранного предмета в блок Info (внизу окна инвентаря).
// Вызывается из show() независимо от того, подключено ли отдельное Info-окно.
void InventoryWindow::renderItemInfo(const std::function<void(Text&&, const Coord&)>& visualizator,
                                     const Coord& parentCd) {
  auto hero = m_hero.lock();
  if (!hero) return;

  std::string desc;
  bool hasItem = false;
  if (auto bag = hero->getBag()) {
    if (auto item = bag->getSelected()) {
      hasItem = true;
      desc = item->toString();
      if (!item->getDescription().empty()) {
        desc += "\n" + item->getDescription();
      }
    }
  }
  // Если ничего не выбрано — показываем характеристики героя (как обычное Info).
  if (!hasItem) {
    const auto& cd = hero->getCoord();
    desc = "pos: " + cd.toString();
    if (auto chars = hero->getChars()) {
      for (int i = 0; i < static_cast<int>(ECharTypes::count); ++i) {
        desc += "\n" + std::string(charNames[i]) + ": " + std::to_string(chars->getValue(i));
      }
    }
  }

  // Блок ниже колонок сумки/частей тела (высота колонок ограничена 18 строками).
  const Coord start{parentCd.x + kBodyPartsX, parentCd.y + 19};
  visualizator(Text("Info:", Color{255, 255, 0}, Color{0, 0, 0}), start);
  // Разбивание по '\n' и печать построчно, обрезая по ширине блока.
  std::istringstream stream(desc);
  std::string line;
  int y = start.y + 1;
  const int maxY = parentCd.y + std::max(m_rectangle.height(), 26);
  while (std::getline(stream, line) && y <= maxY) {
    if (textWidth(line) > 30) {
      line = line.substr(0, 30);
    }
    visualizator(Text(line, Color{255, 255, 255}, Color{0, 0, 0}), Coord{start.x, y});
    ++y;
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
  // Части тела и описание выбранного предмета всегда актуальны (подсветка слотов
  // и описание зависят от того, какой предмет выбран стрелками в сумке).
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
  renderColumn(Coord{parentCd.x + kBodyPartsX, parentCd.y}, m_headerRight, m_itemsRight);

  // Описание выбранного предмета (Info) — ниже колонок, в той же правой колонке.
  renderItemInfo(visualizator, parentCd);
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
