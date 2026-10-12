#include "inventory_window.h"

#include <algorithm>
#include <iterator>

#include "../Bag.h"
#include "../char_names.h"
#include "../char_types.h"
#include "../items/IItems.h"
#include "../units/chars.h"
// #include "window.h"

InventoryWindow::InventoryWindow(const Rectangle& rect) : IWindow(rect) {}

void InventoryWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  Text text{"", Color{255, 255, 255}, Color{0, 0, 0}};
  Coord cd = parentCd + Coord{0, 0};
  for (auto it = m_itemsLeft.begin(); it != m_itemsLeft.end(); ++it) {
    text.m_text = it->m_text;
    cd.y += 1;
    visualizator({it->m_text, Color{255, 255, 255}, Color{0, 0, 0}}, cd);
  }
}

std::string InventoryWindow::getDescriptionText() const {
  auto bag = m_bag.lock();
  auto item = bag ? bag->getSelected() : nullptr;
  if (!item) {
    return "";
  }
  std::string result = "Description:\n_____________\n";
  result += item->toString() + "\n";
  const auto description = item->getDescription();
  if (!description.empty()) {
    result += description + "\n";
  }
  // Характеристики предмета (например, урон/броня), если они есть.
  const std::vector<std::pair<const char*, ESetting>> settings = {
      {"damage", ESetting::damage},
      {"damage1", ESetting::damage1},
      {"damage2", ESetting::damage2},
      {"damage3", ESetting::damage3},
      {"protection", ESetting::protection},
      {"resistance", ESetting::resistance},
      {"abilities", ESetting::abilities},
      {"addAttribute", ESetting::addAttribute},
      {"addBaseChar", ESetting::addBaseChar},
  };
  for (const auto& [settingName, setting] : settings) {
    auto chars = item->getChars(setting);
    if (!chars) {
      continue;
    }
    bool headerPrinted = false;
    for (int i = 0; i < static_cast<int>(ECharTypes::count); ++i) {
      const auto value = chars->getValue(i);
      if (value != 0) {
        if (!headerPrinted) {
          result += std::string(settingName) + ":\n";
          headerPrinted = true;
        }
        result += "  " + charNames[i] + ": " + std::to_string(value) + "\n";
      }
    }
  }
  return result;
}

void InventoryWindow::notify(std::weak_ptr<Publisher> pbag) {
  auto lockPbag = pbag.lock();
  auto count = m_rectangle.height();
  if (lockPbag) {
    auto bag = std::dynamic_pointer_cast<Bag>(lockPbag);
    auto bagItems = bag->showBag(count);
    m_itemsLeft.clear();
    std::copy(bagItems.begin(), bagItems.end(), std::back_inserter(m_itemsLeft));
    m_bag = bag;
  }
}
