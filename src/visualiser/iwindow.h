#ifndef IWINDOW_H
#define IWINDOW_H

#include "../header.h"

struct IWindow {
  Rectangle m_rectangle;
  virtual void empty(){};
};

#endif  // IWINDOW_H
