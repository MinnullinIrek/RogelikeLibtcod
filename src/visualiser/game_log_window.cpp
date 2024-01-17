#include "game_log_window.h"

#include <algorithm>
#include <iterator>

#include "../utils/game_log.h"
#include "window.h"

const Color fromColor = {0, 0, 255};
const Color semicolonColor = {125, 0, 0};
const Color textColor = {125, 125, 125};
const Color bgColor = {0, 0, 0};
const std::string semicolon = ":";

GameLogWindow::GameLogWindow(const Rectangle& rect) : IWindow(rect) {}

void GameLogWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  Text text{"", Color{255, 255, 255}, Color{0, 0, 0}};
  Coord cd = m_rectangle.lu + parentCd + Coord{0, 0};

  for (const auto& message : m_messages) {
    visualizator({message.from, fromColor, bgColor}, cd);
    visualizator({semicolon, semicolonColor, bgColor}, cd + Coord{static_cast<int>(message.from.size()), 0});
    visualizator(
        {message.message, textColor, bgColor}, cd + Coord{static_cast<int>(message.from.size() + semicolon.size()), 0});

    ++cd.y;
  }
}

void GameLogWindow::notify(std::weak_ptr<Publisher> gameLog) {
  auto pgameLog = gameLog.lock();
  if (pgameLog) {
    auto log = std::dynamic_pointer_cast<GameLog>(pgameLog);
    m_messages.clear();
    auto messages = log->getMessages();
    m_messages.insert(m_messages.end(), messages.begin(), messages.end());
  }
}
