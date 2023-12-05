#include "event_logger.h"

#include <cstdarg>
#include <format>
#include <string>

EventLogger::EventLogger(std::string&& fileName) : m_fileName(fileName) {
  // todo create or check file existing
}

EventLogger::~EventLogger() {}

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
