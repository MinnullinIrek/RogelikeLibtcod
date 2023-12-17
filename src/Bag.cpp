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
    if (next > 0 && m_selected != m_items.end()) {
      ++m_selected;
    } else if (next < 0 && m_selected != m_items.begin()) {
      --m_selected;
    }
  }
  emit();
}

bool Bag::contains(std::shared_ptr<IItems> item) { return m_items.find(item) != m_items.end(); }

std::list<Text> Bag::showBag(Count count) {
  auto size = m_items.size();
  Count toStart = static_cast<Count>(std::distance(m_items.begin(), m_selected));
  Count toEnd = m_items.size() - toStart - 1;

  if (size > count) {
    if (toStart > toEnd) {
      toStart = std::min(toStart, count / 2);
      toEnd = count - toStart;
    }
  }
  std::list<Text> bagList;

  auto it = m_selected;
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
