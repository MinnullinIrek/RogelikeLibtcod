#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>

#include "../header.h"

class Window {
 private:
  /* data */
 public:
  Window(/* args */);
  ~Window();

  std::vector<std::string> getText(int start);

  std::string m_text;
  Color m_color;
  Color m_bgColor;
  Rectangle m_rectangle;

 private:
  std::vector<std::string> deliver();
};

#endif  // WINDOW_H
