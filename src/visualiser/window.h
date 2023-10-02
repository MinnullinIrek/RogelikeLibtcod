#ifndef WINDOW_H
#define WINDOW_H

// #include <functional>
#include <string>
#include <vector>

#include "../header.h"
#include "iwindow.h"

class Window : public IWindow {
 public:
  // Window();
  Window(
      std::string_view text = "",
      const Color& color = {255, 255, 255},
      const Color& bgColor = {0, 0, 0},
      const Rectangle& rect = {{0, 0}, {1, 0}});

  virtual ~Window();

  std::vector<std::string> getText(int start = 0);
  int getStart();

  std::string m_text;
  Color m_color;
  Color m_bgColor;

  // Rectangle m_rectangle;
  virtual void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) override;

 private:
  std::vector<std::string> deliver();
  // std::function<std::string()> m_asker;
  int m_start = 0;
};


#endif  // WINDOW_H
