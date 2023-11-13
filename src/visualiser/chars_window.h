#ifndef CHARS_WINDOW_H
#define CHARS_WINDOW_H

#include <list>

#include "../utils/subscriber.h"
#include "iwindow.h"

class CharsWindow : public IWindow, public Subscriber {
 public:
  const std::string name = "charswindow";
  CharsWindow(const Rectangle& rect);
  void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd);
  void notify(std::weak_ptr<Publisher> bag) override;

 private:
  Text m_headerLeft;
  std::list<Text> m_chars;

  Text m_itemDescription;
};

#endif  // CHARS_WINDOW_H
