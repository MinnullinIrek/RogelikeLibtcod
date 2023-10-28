#include "Bag.h"

#include <assert.h>

#include <algorithm>

#include "items/IItems.h"

Bag::Bag() {}
Bag::~Bag() {}
void Bag::putItem(std::shared_ptr<IItems> item, Count count) {
  assert(count >= 0);
  assert(item);

  if (contains(item)) {
    m_items.at(item) += count;
  } else {
    m_items.insert(std::make_pair(item, count));
  }
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

  return popItem;
}

void Bag::select(int next) {
  if (m_items.empty()) {
    if (next > 0 && m_selected != m_items.end()) {
      ++m_selected;
    } else if (next < 0 && m_selected != m_items.begin()) {
      --m_selected;
    }
  }
}

bool Bag::contains(std::shared_ptr<IItems> item) { return m_items.find(item) != m_items.end(); }

std::list<Text> Bag::showBag(Count count) {
  auto size = m_items.size();

  Count toStart = static_cast<Count>(std::distance(m_items.begin(), m_selected));
  Count toEnd = static_cast<Count>(std::distance(m_items.end(), m_selected));

  if (size > count) {
    if (toStart > toEnd) {
      toStart = std::min(toStart, count / 2);
      toEnd = count - toStart;
    }
  }
  std::string sbag;
  std::list<Text> bagList;

  // std::for_each(m_items.begin(), m_items.end(), [&sbag](const auto& itemPair){ itemPair.fi })
  auto it = m_selected;
  for (auto i = toStart; i > 0; --i) {
    --it;
  };
  for (auto i = 0; i < toStart + toEnd; ++i, ++it) {
    if (it == m_selected) {
      sbag += "[#] ";
    } else {
      sbag += "[ ] ";
    }
    std::shared_ptr<IItems> item = it->first;

    sbag += item->toString() + std::to_string(it->second);
    sbag += '\n';
    
    bagList.emplace_back(Text(sbag, Color{255, 255, 255}, Color{0, 0, 0}));
  }
  return bagList;
}
