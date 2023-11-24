#include "chars_window.h"

#include <algorithm>

#include "../char_names.h"
#include "../char_types.h"
#include "../units/chars.h"

CharsWindow::CharsWindow(const Rectangle& rect) : IWindow(rect) {}

void CharsWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  Coord cd = parentCd + Coord{1, 3};
  std::for_each(std::begin(m_charName), std::end(m_charName), [&cd, &visualizator](const auto& text) {
    visualizator(Text(text, {255, 255, 255}, {0, 0, 0}), cd);
    cd.y += 1;
  });

  cd = parentCd + Coord{15, 3};
  std::for_each(std::begin(m_charValue), std::end(m_charValue), [&cd, &visualizator](const auto& text) {
    visualizator(Text(text, {255, 255, 255}, {0, 0, 0}), cd);
    cd.y += 1;
  });
}

void CharsWindow::notify(std::weak_ptr<Publisher> publisher) {
  auto locked = publisher.lock();
  auto chars = std::dynamic_pointer_cast<Chars>(locked);
  if (chars) {
    for (int i = 0; i < static_cast<int>(ECharTypes::count); ++i) {
      m_charName.emplace_back(charNames[i]);
      m_charValue.emplace_back(std::to_string(chars->getValue(i)));
    }
    /*m_charName.emplace_back("-----------------");
    m_charValue.emplace_back("---");
    for (int i = static_cast<int>(ECharTypes::count); i < static_cast<int>(EDamageType::count); ++i) {
      m_charName.emplace_back(charNames[i]);
      m_charValue.emplace_back(std::to_string(chars->getValue(i)));
    }*/
  }
}
