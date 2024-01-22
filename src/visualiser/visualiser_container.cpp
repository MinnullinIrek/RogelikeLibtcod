#include "visualiser_container.h"

#include <algorithm>

void VisualiserContainer::show() const {
  std::for_each(std::begin(m_visulisers), std::end(m_visulisers), [](std::shared_ptr<VisualiserInterface> interface) {
    interface->show();
  });
}
void VisualiserContainer::addWindow(std::shared_ptr<IWindow> window) {
  std::for_each(
      std::begin(m_visulisers), std::end(m_visulisers), [window](std::shared_ptr<VisualiserInterface> interface) {
        interface->addWindow(window);
      });
}
void VisualiserContainer::addVisualiser(std::shared_ptr<VisualiserInterface> visualiser) {
  m_visulisers.emplace_back(visualiser);
}
