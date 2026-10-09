#ifndef LOGGER_H
#define LOGGER_H

#include <cstdarg>
#include <string_view>

struct Logger {
  static void log(std::string_view filename, std::string_view function, int line, const char* format, ...);
  static void vlog(std::string_view filename, std::string_view function, int line, const char* format, va_list args);
};

#define LOG(...) Logger::log(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#endif
