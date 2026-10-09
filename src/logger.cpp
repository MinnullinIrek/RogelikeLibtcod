#include "logger.h"

#include <cstring>
#include <vector>

void Logger::vlog(std::string_view filename, std::string_view function, int line, const char* format, va_list args) {
  // Предварительная оценка размера буфера под отформатированное сообщение.
  std::size_t capacity = std::strlen(format) + filename.size() + function.size() + 32;
  std::vector<char> buffer(capacity);

  va_list copy;
  va_copy(copy, args);
  int written = std::vsnprintf(buffer.data(), buffer.size(), format, copy);
  va_end(copy);

  if (written < 0) {
    return;
  }
  // vsnprintf вернул число символов, которые были бы записаны при большем буфере — расширяем и повторяем.
  if (static_cast<std::size_t>(written) >= buffer.size()) {
    buffer.resize(static_cast<std::size_t>(written) + 1);
    va_copy(copy, args);
    written = std::vsnprintf(buffer.data(), buffer.size(), format, copy);
    va_end(copy);
    if (written < 0) {
      return;
    }
  }

  // %.*s безопасен для string_view (он не гарантированно NUL-терминирован).
  std::printf("%.*s:%d: ", static_cast<int>(filename.size()), filename.data(), line);
  std::printf("%.*s: ", static_cast<int>(function.size()), function.data());
  std::fwrite(buffer.data(), 1, static_cast<std::size_t>(written), stdout);
  std::fflush(stdout);
}

void Logger::log(std::string_view filename, std::string_view function, int line, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vlog(filename, function, line, format, args);
  va_end(args);
}
