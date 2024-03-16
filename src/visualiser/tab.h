#ifndef TAB_H
#define TAB_H

#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "../header.h"
#include "iwindow.h"
#include "shower.h"

class Window;
using WindowKey = int;

class Tab : /*public Shower,*/ public IWindow {
 public:
  Tab(std::string_view name, std::function<void(Text&& text, const Coord& cd)>&& shower, const Rectangle& r = {{0,0}, {0,0}});
  virtual ~Tab();
  std::string_view getName();
  WindowKey addWindow(std::shared_ptr<IWindow> window);
  std::shared_ptr<IWindow> getWindow(WindowKey key, bool pop = false);

 public:  // overrided
  virtual void show(const std::function<void(Text&&, const Coord&)>& visualizator,
    const Coord& parentCd) const override;

 private:
  void printWindow(std::shared_ptr<Window> window, const Coord& cdParent,
      const std::function<void(Text&&, const Coord&)>& visualizator);
  // Rectangle m_rect;
  const std::string m_name;
  std::unordered_map<WindowKey, std::shared_ptr<IWindow>> m_windows;
  unsigned int m_count = 0;
  ;
  std::function<void(Text&& text, const Coord& cd)> m_shower;
};

#endif
