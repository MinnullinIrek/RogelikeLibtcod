#include "window.h"

#include <assert.h>
#include <math.h>

Window::Window(/* args */) {}

Window::~Window() {}

std::vector<std::string> Window::deliver() {
  std::vector<std::string> delivered;
  const int length = m_rectangle.rd.x - m_rectangle.lu.x;

  for (int i = 0;;) {
    int iRight = __min(length, m_text.length() - i);

    delivered.push_back(m_text.substr(i, iRight));
    if (i >= m_text.length()) {
      break;
    } else {
      i += iRight;
    }
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

  for (int i = start; i < __min(length + start, texts.size() + start); ++i) {
    finalTexts.push_back(texts.at(i));
  }
  return finalTexts;
}
