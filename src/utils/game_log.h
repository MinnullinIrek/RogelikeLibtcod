#ifndef GAME_LOG_H
#define GAME_LOG_H

#include <functional>
#include <list>
#include <string>

#include "../header.h"
#include "game_log_message.h"
#include "subscriber.h"

class GameLog : public Publisher {
 public:
  GameLog(std::function<std::list<GameLogMessage>(int)>&& dbReader, std::function<void(GameLogMessage&&)>&& dbWriter);
  ~GameLog();

  void putMessage(GameLogMessage&& message);
  const std::list<GameLogMessage>& getMessages() const;

 private:
  void syncDb();
  void sendMessageToDb();

 private:
  std::list<GameLogMessage> m_messages;
  const int m_bufferCount;
  std::function<std::list<GameLogMessage>(int)> m_dbReader;
  std::function<void(GameLogMessage&&)> m_dbWriter;
};

#endif  // Game_Log_H
