#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include <condition_variable>
#include <format>
#include <fstream>
#include <list>
#include <string_view>
#include <thread>

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
    m_messages.push_back({source, std::format(arg, args...)});
    cv.notify_one();
  }

  void write();
  void turn_off();

 private:
  std::thread m_writingThread;
  std::condition_variable m_cv;
  std::string m_fileName;
  std::list<Message> m_messages;
  std::fstream m_file;
  std::mutex m_mutex;
  bool m_exit;
};

#endif
