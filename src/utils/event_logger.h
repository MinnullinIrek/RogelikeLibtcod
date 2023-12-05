#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <format>
#include <list>
#include <string_view>

struct Message {
  std::string source;
  std::string text;
};

class EventLogger {
 public:
  EventLogger(std::string&& fileName);
  ~EventLogger();
  template <typename Arg, typename... Args>
  void log(std::string_view source, const Arg& arg, const Args&... args) {
    auto str = std::format(arg, args...);
  }

 private:
  std::string m_fileName;
  std::list<Message> m_messages;
};

#endif
