#ifndef INVENTORY_WINDOW_H
#define INVENTORY_WINDOW_H

#include <list>
#include <memory>

#include "../utils/subscriber.h"
#include "iwindow.h"

class Unit;

// Окно инвентаря: слева — сумка, справа — экипированные предметы по частям тела.
// Подписка и на Bag, и на героя (Unit/BodyParts каскадно шлют emit), поэтому
// список надетых вещей обновляется сразу после equip/unequip.
class InventoryWindow : public IWindow, public Subscriber {
 public:
  const std::string name = "inventorywindow";
  InventoryWindow(const Rectangle& rect);
  void show(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) override;
  void notify(std::weak_ptr<Publisher> publisher) override;
  void setHero(std::shared_ptr<Unit> hero);

 private:
  Text m_headerLeft;
  std::list<Text> m_itemsLeft;

  Text m_headerRight;
  std::list<Text> m_itemsRight;

  std::weak_ptr<Unit> m_hero;
  Text m_itemDescription;
};

#endif  // INVENTORY_WINDOW_H
