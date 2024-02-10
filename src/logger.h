#ifndef LOGGER_H
#define LOGGER_H

#include <string_view>

struct Logger {
 static void log(std::string_view filename, std::string_view function, int line, char* format, ...);
};

#define LOG(format, ...) Logger::log(__FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__);

#endif

