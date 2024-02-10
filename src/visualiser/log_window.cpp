#include "log_window.h"

#include <algorithm>

LogWindow::LogWindow(const Rectangle& rect) : IWindow(rect) {}

void LogWindow::show(const std::function<void(Text&&, const Coord&)>& /*visualizator*/, const Coord& /*parentCd*/) {}

void LogWindow::notify(std::weak_ptr<Publisher> /*publisher*/) {}
