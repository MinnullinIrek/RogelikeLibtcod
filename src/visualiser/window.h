#ifndef WINDOW_H
#define WINDOW_H

// #include <functional>
#include <string>
#include <vector>

#include "../header.h"
#include "iwindow.h"

class Window : public IWindow {
 private:
  /* data */
 public:
  Window();
  virtual ~Window();

  std::vector<std::string> getText(int start = 0);
  int getStart();

  std::string m_text;
  Color m_color;
  Color m_bgColor;

  // Rectangle m_rectangle;

 private:
  std::vector<std::string> deliver();
  // std::function<std::string()> m_asker;
  int m_start = 0;
};

#endif  // WINDOW_H
