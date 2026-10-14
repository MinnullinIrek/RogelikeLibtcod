#include "Bag.h"

#include <assert.h>

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "char_names.h"
#include "char_types.h"
#include "items/IItems.h"
#include "units/chars.h"

Bag::Bag() {}
Bag::~Bag() {}
void Bag::putItem(std::shared_ptr<IItems> item) {
  assert(item);
  const auto wasEmpty = m_items.empty();
  m_items.push_back(item);
  if (wasEmpty) {
    m_selected = 0;
  }
  emit();
}

std::weak_ptr<IItems> Bag::popItem() {
  assert(!m_items.empty());
  assert(m_selected >= 0 && m_selected < static_cast<int>(m_items.size()));

  std::weak_ptr<IItems> popItem = m_items[m_selected];
  // Удаление за O(1): меняем удаляемый элемент местами с последним и убираем хвост.
  const auto last = static_cast<int>(m_items.size()) - 1;
  std::swap(m_items[m_selected], m_items[last]);
  m_items.pop_back();
  if (m_selected >= static_cast<int>(m_items.size())) {
    // Выделение ушло за конец — сдвигаем назад.
    m_selected = static_cast<int>(m_items.size()) - 1;
  }
  emit();
  return popItem;
}

void Bag::select(int next) {
  if (!m_items.empty()) {
    // Если ничего не выделено (например, при первом открытии инвентаря),
    // выделяем первую вещь сверху.
    if (m_selected < 0 || m_selected >= static_cast<int>(m_items.size())) {
      m_selected = 0;
    } else if (next > 0) {
      if (m_selected + 1 < static_cast<int>(m_items.size())) {
        ++m_selected;
      }
    } else if (next < 0) {
      if (m_selected > 0) {
        --m_selected;
      }
    }
  }
  emit();
}

bool Bag::contains(std::shared_ptr<IItems> item) {
  return std::find(m_items.begin(), m_items.end(), item) != m_items.end();
}

std::pair<std::list<Text>, Bag::ItemDescription> Bag::showBag(Count count) {
  std::list<Text> bagList;
  ItemDescription description;
  if (m_items.empty()) {
    return {bagList, description};
  }
  const int size = static_cast<int>(m_items.size());
  // Выделение не должно теряться: если оно некорректное (первое открытие инвентаря),
  // выделяем первую вещь сверху.
  if (m_selected < 0 || m_selected >= size) {
    m_selected = 0;
  }

  const int visible = std::min(static_cast<int>(count), size);
  // Выделение всегда в верхней строке окна, пока под ним хватает строк до
  // конца инвентаря. Когда достигаются последние предметы, окно «прилипает»
  // к концу списка (ниже последнего элемента оно не уходит) — дальше при
  // навигации перемещается только выделение [#], а сами предметы стоят на месте.
  int first = m_selected;
  if (first > size - visible) {
    first = size - visible;
  }
  if (first < 0) {
    first = 0;
  }

  for (int i = 0; i < visible; ++i) {
    const int idx = first + i;
    std::shared_ptr<IItems> item = m_items[idx];
    std::string sbag = ((idx == m_selected) ? "[#] " : "[ ] ") + item->toString();

    bagList.emplace_back(Text(sbag, Color{255, 255, 255}, Color{0, 0, 0}));
  }

  // Описание выбранного предмета.
  const auto& selected = m_items[m_selected];
  description["name"] = selected->toString();
  const auto itemDescription = selected->getDescription();
  if (!itemDescription.empty()) {
    description["description"] = itemDescription;
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
    auto chars = selected->getChars(setting);
    if (!chars) {
      continue;
    }
    std::string settingText;
    for (int i = 0; i < static_cast<int>(ECharTypes::count); ++i) {
      const auto value = chars->getValue(i);
      if (value != 0) {
        if (settingText.empty()) {
          settingText += std::string(settingName) + ":\n";
        }
        settingText += "  " + charNames[i] + ": " + std::to_string(value) + "\n";
      }
    }
    if (!settingText.empty()) {
      description[settingName] = settingText;
    }
  }
  return {bagList, description};
}

void Bag::putItem(std::shared_ptr<Bag> bag) {
  for (const auto& item : bag->m_items) {
    putItem(item);
  }
  emit();
}
