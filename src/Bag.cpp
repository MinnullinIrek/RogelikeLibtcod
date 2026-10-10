#include "Bag.h"
#include <assert.h>
#include <algorithm>
#include "items/IItems.h"

Bag::Bag() : m_selected(m_items.end()) {}
Bag::~Bag() {}
void Bag::putItem(std::shared_ptr<IItems> item, Count count) {
  putInsert(item, count);
  emit();
}

std::pair<std::weak_ptr<IItems>, Count> Bag::popItem(Count popCount) {
  assert(!m_items.empty());
  assert(m_selected->second > 0);

  auto& popItem = *m_selected;
  popItem.second = std::min(popCount, popItem.second);

  m_selected->second -= popCount;
  if (m_selected->second <= 0) {
    m_selected = m_items.erase(m_selected);
  }
  if (m_items.empty()) {
    m_selected = m_items.end();
  }
  emit();
  return popItem;
}

void Bag::select(int next) {
  if (!m_items.empty()) {
    // Если ничего не выделено (например, при первом открытии инвентаря),
    // выделяем первую вещь сверху.
    if (m_selected == m_items.end()) {
      m_selected = m_items.begin();
    } else if (next > 0) {
      auto nextIt = std::next(m_selected);
      if (nextIt != m_items.end()) {
        ++m_selected;
      }
    } else if (next < 0) {
      if (m_selected != m_items.begin()) {
        --m_selected;
      }
    }
  }
  emit();
}

bool Bag::contains(std::shared_ptr<IItems> item) { return m_items.find(item) != m_items.end(); }

std::list<Text> Bag::showBag(Count count) {
  std::list<Text> bagList;
  if (m_items.empty()) {
    return bagList;
  }
  // Выделение не должно теряться: если оно пустое (первое открытие инвентаря),
  // выделяем первую вещь сверху.
  if (m_selected == m_items.end()) {
    m_selected = m_items.begin();
  }

  Count toStart = static_cast<Count>(std::distance(m_items.begin(), m_selected));
  Count toEnd = static_cast<Count>(m_items.size()) - toStart - 1;

  // В окно входит: toStart строк над выделенной + выделенная + toEnd строк под ней.
  if (toStart + 1 + toEnd > count) {
    if (toStart > toEnd) {
      toStart = std::min(toStart, std::max(0, (count - 1) / 2));
      toEnd = count - toStart - 1;
    } else {
      toEnd = std::min(toEnd, std::max(0, count - 2));
      toStart = count - toEnd - 1;
    }
  }

  auto it = m_items.end();
  for (auto i = toStart; i > 0; --i) {
    --it;
  };
  for (auto i = 0; i < toStart + toEnd && it != m_items.end(); ++i, ++it) {
    std::shared_ptr<IItems> item = it->first;
    std::string sbag =
        ((it == m_selected) ? "[#] " : "[ ] ") + item->toString() + "[" + std::to_string(it->second) + "]";

    bagList.emplace_back(Text(sbag, Color{255, 255, 255}, Color{0, 0, 0}));
  }
  return bagList;
}

void Bag::putItem(std::shared_ptr<Bag> bag) {
  for (const auto& itemCount : bag->m_items) {
    putInsert(itemCount.first, itemCount.second);
  }
  emit();
}

void Bag::putInsert(std::shared_ptr<IItems> item, Count count) {
  auto wasEmpty = m_items.empty();
  assert(count >= 0);
  assert(item);

  if (contains(item)) {
    m_items.at(item) += count;
  } else {
    m_items.insert(std::make_pair(item, count));
  }
  if (wasEmpty && (!m_items.empty())) {
    m_selected = m_items.begin();
  }
}
