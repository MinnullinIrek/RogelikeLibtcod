#include "main_window.h"

#include <assert.h>

#include "../logger.h"

MainWindow::MainWindow(const Rectangle& rect) : IWindow(rect) {}

MainWindow::~MainWindow() {}

void MainWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  // LOG("\nMainWindow::show start [%d]", (int)m_currentWindowType);
  auto currentWindow = m_windows.at(m_currentWindowType);
  // LOG("\nMainWindow::show");
  assert(currentWindow);
  currentWindow->show(visualizator, parentCd);

  // Сторонние окна (например Info) печатаем, только если активный режим не запрещает их.
  for (const auto& [whenHide, window] : m_hiddenWindows) {
    if (m_currentWindowType != whenHide) {
      window->show(visualizator, {0, 0});
    }
  }
  // LOG("\nMainWindow::show end");
}
void MainWindow::notify(std::weak_ptr<Publisher> /*publisher*/) {}
void MainWindow::addWindow(EMainWindows eWindowType, std::shared_ptr<IWindow> window) {
  m_windows[eWindowType] = window;
}

void MainWindow::addHideWindow(EMainWindows whenCurrent, std::shared_ptr<IWindow> windowToHide) {
  m_hiddenWindows.emplace_back(whenCurrent, windowToHide);
}

void MainWindow::setCurrent(EMainWindows currentWindowType) { m_currentWindowType = currentWindowType; }
