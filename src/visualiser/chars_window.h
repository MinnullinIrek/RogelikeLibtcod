#ifndef CHARS_WINDOW_H
#define CHARS_WINDOW_H

#include <vector>

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
  std::vector<std::string> m_charName;
  std::vector<std::string> m_charValue;

  Text m_itemDescription;
};

#endif  // CHARS_WINDOW_H
