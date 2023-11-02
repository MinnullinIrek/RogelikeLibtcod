#include "main_window.h"

MainWindow::MainWindow(const Rectangle& rect) : IWindow(rect) {}

MainWindow::~MainWindow() {}

void MainWindow::show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) {
  auto currentWindow = m_windows.at(m_currentWindowType);
  currentWindow->show(visualizator, parentCd);
}
void MainWindow::notify(std::weak_ptr<Publisher> publisher) {}
void MainWindow::addWindow(EMainWindows eWindowType, std::shared_ptr<IWindow> window) {
  m_windows[eWindowType] = window;
}
