#include "chars_window.h"

#include "../char_types.h"
#include "../units/chars.h"

CharsWindow::CharsWindow(const Rectangle& rect) : IWindow(rect) {}

void CharsWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {}

void CharsWindow::notify(std::weak_ptr<Publisher> publisher) {
  auto locked = publisher.lock();
  auto chars = std::dynamic_pointer_cast<Chars>(locked);
  if (chars) {
    for (int i = 0; i < static_cast<int>(ECharTypes::count); ++i) {
      auto val = chars->getValue(i);
    }
  }
}
