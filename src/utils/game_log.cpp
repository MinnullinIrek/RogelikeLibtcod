#include "game_log.h"

#include <assert.h>

#include "consts_reader.h"

GameLog::GameLog(
    std::function<std::list<GameLogMessage>(int)>&& dbReader, std::function<void(GameLogMessage&&)>&& dbWriter)
    : Publisher(), m_bufferCount(SI("gameLogBufferCount")), m_dbReader(dbReader), m_dbWriter(dbWriter) {
  m_messages = m_dbReader(m_bufferCount);
}

GameLog::~GameLog() { syncDb(); }

void GameLog::putMessage(GameLogMessage&& message) {
  m_messages.emplace_back(message);
  if (m_messages.size() > m_bufferCount) {
    sendMessageToDb();
  }
  emit();
}

void GameLog::syncDb() {
  while (!m_messages.empty()) {
    sendMessageToDb();
  }
}

void GameLog::sendMessageToDb() {
  assert(!m_messages.empty());
  m_dbWriter(std::move(m_messages.front()));
  m_messages.pop_front();
}

const std::list<GameLogMessage>& GameLog::getMessages() const { return m_messages; }
