#ifndef INVENTORY_WINDOW_H
#define INVENTORY_WINDOW_H

#include <list>

#include "../utils/subscriber.h"
#include "iwindow.h"

class InventoryWindow : public IWindow, public Subscriber {
 public:
  const std::string name = "inventorywindow";
  InventoryWindow(const Rectangle& rect);
  void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) const;
  void notify(std::weak_ptr<Publisher> bag) override;

 private:
  Text m_headerLeft;
  std::list<Text> m_itemsLeft;

  //  Text m_headerRight;
  //  std::vector<Text> m_itemsRight;

  Text m_itemDescription;
};

#endif  // INVENTORY_WINDOW_H
