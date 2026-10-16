#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <unordered_map>
#include <vector>

#include "../header.h"
#include "../utils/subscriber.h"
#include "iwindow.h"

enum class EMainWindows { emap, einventory, echars };

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
  // Скрыть стороннее окно (например Info), когда активен указанный режим.
  void addHideWindow(EMainWindows whenCurrent, std::shared_ptr<IWindow> windowToHide);

 private:
  EMainWindows m_currentWindowType = EMainWindows::emap;
  std::unordered_map<EMainWindows, std::shared_ptr<IWindow>> m_windows;
  std::vector<std::pair<EMainWindows, std::shared_ptr<IWindow>>> m_hiddenWindows;
};

#endif  // MAIN_WINDOW_H
