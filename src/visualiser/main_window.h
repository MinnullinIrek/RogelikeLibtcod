#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <unordered_map>

#include "../header.h"
#include "../utils/subscriber.h"
#include "iwindow.h"

enum class EMainWindows { emap, einventory };

class MainWindow : public IWindow, public Subscriber {
 private:
  /* data */
 public:
  const std::string name = "mainwindow";
  MainWindow(const Rectangle& rect);
  ~MainWindow();
  virtual void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) override;
  void notify(std::weak_ptr<Publisher> publisher) override;
  void addWindow(EMainWindows eWindowType, std::shared_ptr<IWindow>);
  void setCurrent(EMainWindows currentWindowType);

 private:
  EMainWindows m_currentWindowType = EMainWindows::emap;
  std::unordered_map<EMainWindows, std::shared_ptr<IWindow>> m_windows;
};

#endif  // MAIN_WINDOW_H
