#include "inventory_window.h"

#include <algorithm>
#include <iterator>
#include <sstream>

#include "../Bag.h"
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
  if (m_selectedItemDescription.empty()) {
    return "";
  }
  std::ostringstream result;
  result << "Description:\n_____________\n";
  // Сначала имя предмета, затем описание, затем характеристики.
  if (const auto it = m_selectedItemDescription.find("name"); it != m_selectedItemDescription.end()) {
    result << it->second << "\n";
  }
  if (const auto it = m_selectedItemDescription.find("description"); it != m_selectedItemDescription.end()) {
    result << it->second << "\n";
  }
  for (const auto& [key, value] : m_selectedItemDescription) {
    if (key == "name" || key == "description") {
      continue;
    }
    result << value;
  }
  return result.str();
}

void InventoryWindow::notify(std::weak_ptr<Publisher> pbag) {
  auto lockPbag = pbag.lock();
  auto count = m_rectangle.height();
  if (lockPbag) {
    auto bag = std::dynamic_pointer_cast<Bag>(lockPbag);
    auto [bagItems, selectedItemDescription] = bag->showBag(count);
    m_itemsLeft.clear();
    std::copy(bagItems.begin(), bagItems.end(), std::back_inserter(m_itemsLeft));
    m_selectedItemDescription = std::move(selectedItemDescription);
  }
}
