#include "window.h"

#include <assert.h>
#include <math.h>

Window::Window(
    std::function<int(std::weak_ptr<Publisher>, std::string&, Color&, Color&)>&& updater,
    std::string_view text,
    const Color& color,
    const Color& bgColor,
    const Rectangle& rect,
    bool needDeliver)
    : m_text(text), m_color(color), m_bgColor(bgColor), IWindow(rect), m_updater(updater), m_needDeliver(needDeliver) {}

void Window::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  auto texts = getText(0);
  for (const auto& text : texts) {
    visualizator(Text(text, m_color, m_bgColor), parentCd + m_rectangle.lu);
  }
}

Window::~Window() {}

std::vector<std::string> Window::deliver() {
  std::vector<std::string> delivered;
  if (m_needDeliver) {
    const int length = m_rectangle.rd.x - m_rectangle.lu.x;

    for (int i = 0;;) {
      int iRight = __min(length, static_cast<int>(m_text.length()) - i);

      delivered.push_back(m_text.substr(i, iRight));
      if (i >= m_text.length()) {
        break;
      } else {
        i += iRight;
      }
    }
  } else {
    delivered.push_back(m_text);
  }

  return delivered;
}

std::vector<std::string> Window::getText(int start) {
  std::vector<std::string> finalTexts;
  auto texts = deliver();
  const int length = m_rectangle.rd.y - m_rectangle.lu.y;
  if (texts.size() <= length) {
    start = 0;
  }
  assert(start < texts.size());

  m_start = start;

  for (int i = start; i < __min(length + start, texts.size() + start); ++i) {
    finalTexts.push_back(texts.at(i));
  }
  return finalTexts;
}

int Window::getStart() { return m_start; }

void Window::notify(std::weak_ptr<Publisher> publisher) {
  auto result = m_updater(publisher, m_text, m_color, m_bgColor);
  if (result != 0) {
    throw("void Window::notify wrong publisher");
  }
}
