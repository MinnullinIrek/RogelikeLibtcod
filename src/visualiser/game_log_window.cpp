#include "game_log_window.h"

#include <algorithm>
#include <iterator>

#include "../utils/game_log.h"
#include "window.h"

GameLogWindow::GameLogWindow(const Rectangle& rect) : IWindow(rect) {}

void GameLogWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  Text text{"", Color{255, 255, 255}, Color{0, 0, 0}};
  auto start = m_itemsLeft.begin();
  Coord cd = parentCd + Coord{5, 3};
  for (auto it = start; it != m_itemsLeft.end(); ++it) {
    text.m_text = it->m_text;
    cd.y += 1;
    visualizator({it->m_text, Color{255, 255, 255}, Color{0, 0, 0}}, cd);
  }
}

void GameLogWindow::notify(std::weak_ptr<Publisher> gameLog) {
  auto pgameLog = gameLog.lock();
  // auto count = m_rectangle.height();
  // if (lockPbag) {
  //   auto bag = std::dynamic_pointer_cast<Bag>(lockPbag);
  //   auto bagItems = bag->showBag(count);
  //   m_itemsLeft.clear();
  //   std::copy(bagItems.begin(), bagItems.end(), std::back_inserter(m_itemsLeft));
  // }
}
