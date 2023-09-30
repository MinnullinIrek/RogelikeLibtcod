#include "tab.h"

#include <assert.h>

#include "window.h"

Tab::Tab(std::string_view name, std::function<void(Text&& text, const Coord& cd)>&& shower)
    : m_name(name), m_shower(shower) {
  m_rectangle.lu = {0, 0};
  m_rectangle.rd = {100, 100};
}

Tab::~Tab() {}

std::string_view Tab::getName() { return m_name; }

WindowKey Tab::addWindow(std::shared_ptr<IWindow> window) {
  assert(window);
  ++m_count;
  //m_windows.insert(m_count, window);
  m_windows[m_count] = window;

  
  return m_count;
}

std::shared_ptr<IWindow> Tab::getWindow(WindowKey key, bool pop) {
  auto ret = m_windows.at(key);
  if (pop) {
    m_windows.erase(key);
  }
  return ret;
}

void Tab::show(const std::function<void(Text&&, const Coord&)>& visualizator,
  const Coord& parentCd) {
  for (auto& iwindow : m_windows) {
    auto tab = std::dynamic_pointer_cast<Tab>(iwindow.second);
    if (tab) {
      tab->show(visualizator, parentCd + m_rectangle.lu);
    } else {
      auto window = std::dynamic_pointer_cast<Window>(iwindow.second);
      printWindow(window, parentCd, visualizator);
    }
  }
}

void Tab::printWindow(std::shared_ptr<Window> window, const Coord& cdParent,
    const std::function<void(Text&&, const Coord&)>& visualizator) {
  auto startCd = cdParent + window->m_rectangle.lu + m_rectangle.lu;
  auto texts = window->getText();

  for (const auto& text : texts) {
    visualizator(Text(text, window->m_color, window->m_bgColor), startCd);
  }
}

