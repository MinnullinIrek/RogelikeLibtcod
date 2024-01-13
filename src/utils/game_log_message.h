#ifndef GAME_LOG_MESSAGE_H
#define GAME_LOG_MESSAGE_H

#include <string>

#include "../header.h"


struct GameLogMessage {
  TimeType time;
  std::string from;
  std::string message;
};

#endif  // GAME_LOG_MESSAGE_H
