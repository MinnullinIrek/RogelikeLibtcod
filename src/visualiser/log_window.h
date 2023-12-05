#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H
#include <vector>

#include "../utils/subscriber.h"
#include "iwindow.h"

class LogWindow : public IWindow, public Subscriber {
 public:
  const std::string name = "logwindow";
  LogWindow(const Rectangle& rect);
  void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd);
  void notify(std::weak_ptr<Publisher> bag) override;

 private:
  Text m_header;
  std::vector<std::string> m_charValue;
};

#endif
