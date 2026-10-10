#include "Bag.h"
#include <assert.h>
#include <algorithm>
#include "items/IItems.h"

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

std::list<Text> Bag::showBag(Count count) {
  std::list<Text> bagList;
  if (m_items.empty()) {
    return bagList;
  }
  const int size = static_cast<int>(m_items.size());
  // Выделение не должно теряться: если оно некорректное (первое открытие инвентаря),
  // выделяем первую вещь сверху.
  if (m_selected < 0 || m_selected >= size) {
    m_selected = 0;
  }

  int toStart = m_selected;
  int toEnd = size - toStart - 1;

  // В окно входит: toStart строк над выделенной + выделенная + toEnd строк под ней.
  if (toStart + 1 + toEnd > count) {
    if (toStart > toEnd) {
      toStart = std::min(toStart, std::max(0, (static_cast<int>(count) - 1) / 2));
      toEnd = static_cast<int>(count) - toStart - 1;
    } else {
      toEnd = std::min(toEnd, std::max(0, static_cast<int>(count) - 2));
      toStart = static_cast<int>(count) - toEnd - 1;
    }
  }

  const int first = m_selected - toStart;
  for (int i = 0; i < toStart + toEnd + 1; ++i) {
    const int idx = first + i;
    if (idx < 0 || idx >= size) {
      continue;
    }
    std::shared_ptr<IItems> item = m_items[idx];
    std::string sbag = ((idx == m_selected) ? "[#] " : "[ ] ") + item->toString();

    bagList.emplace_back(Text(sbag, Color{255, 255, 255}, Color{0, 0, 0}));
  }
  return bagList;
}

void Bag::putItem(std::shared_ptr<Bag> bag) {
  for (const auto& item : bag->m_items) {
    putItem(item);
  }
  emit();
}
