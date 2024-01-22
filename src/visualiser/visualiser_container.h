#ifndef VISUALISER_CONTAINER_H
#define VISUALISER_CONTAINER_H

#include <memory>
#include <vector>

#include "../header.h"
#include "visualiser_interface.h"

class VisualiserContainer : public VisualiserInterface {
 public:
  VisualiserContainer() = default;
  ~VisualiserContainer() = default;
  void show() const override;
  void addWindow(std::shared_ptr<IWindow> window) override;
  void addVisualiser(std::shared_ptr<VisualiserInterface> visualiser);

 private:
  std::vector<std::shared_ptr<VisualiserInterface>> m_visulisers;
};

#endif
