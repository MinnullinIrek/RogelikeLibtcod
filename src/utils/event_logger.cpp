#include "event_logger.h"

#include <cstdarg>
#include <filesystem>
//#include <format>
// #include "fmt/format.h"
#include <string>

EventLogger::EventLogger(std::string&& fileName) : m_fileName(fileName), m_exit(false) {
  m_file.open("test.txt", std::ios::out);
  m_writingThread = std::thread(&EventLogger::write, this);
}

EventLogger::~EventLogger() { m_file.close(); }

void EventLogger::write() {
  while (true) {
    if (!m_messages.empty()) {
      const auto& message = m_messages.front();
      m_file << message.source << ":" << message.text << "\n";
      m_messages.pop_front();
    } else {
      if (m_exit) {
        break;
      } else {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk);
      }
    }
  }
}

void EventLogger::turn_off() { m_writingThread.join(); }

// void EventLogger::log(std::string_view source, std::string_view format, ...) {
// std::format(format, ...);

// char* sMessage = nullptr;
// size_t nLength;
// va_list args;
// va_start(args, form);
// nLength = _vscprintf(format, args) + 1;
// sMessage = new char[nLength];
// vsprintf_s(sMessage, nLength, format, args);
// std::string text(sMessage);
// delete[] sMessage;

// };
