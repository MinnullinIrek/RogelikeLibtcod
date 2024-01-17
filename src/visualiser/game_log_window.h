#ifndef GAME_LOG_WINDOW_H
#define GAME_LOG_WINDOW_H

#include <list>

#include "../utils/game_log_message.h"
#include "../utils/subscriber.h"
#include "iwindow.h"

class GameLogWindow : public IWindow, public Subscriber {
 public:
  const std::string name = "gamelogwindow";
  GameLogWindow(const Rectangle& rect);
  void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd);

 private:
  void notify(std::weak_ptr<Publisher> gameLog) override;

 private:
  Text m_headerLeft;
  std::vector<GameLogMessage> m_messages;

  Text m_itemDescription;
};

#endif  // GAME_LOG_WINDOW_H
