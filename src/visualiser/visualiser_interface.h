#ifndef VISUALISER_INTERFACE_H
#define VISUALISER_INTERFACE_H

#include <memory>
class IWindow;

class VisualiserInterface {
 public:
  virtual void show() const = 0;
  virtual void addWindow(std::shared_ptr<IWindow> window) = 0;
};

#endif  // VISUALISER_INTERFACE_H
