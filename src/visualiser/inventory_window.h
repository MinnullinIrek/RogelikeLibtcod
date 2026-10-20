#ifndef INVENTORY_WINDOW_H
#define INVENTORY_WINDOW_H

#include <list>
#include <memory>
#include <string>

#include "../utils/subscriber.h"
#include "iwindow.h"

class Unit;
class Window;

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
  // Окно Info (описание выбранного предмета). InventoryWindow обновляет его текст
  // при навигации по сумке; в режиме инвентаря Info опущен ниже колонок (main.cpp).
  void setInfoWindow(std::shared_ptr<Window> info) { m_infoWindow = info; }

 private:
  Text m_headerLeft;
  std::list<Text> m_itemsLeft;

  Text m_headerRight;
  std::list<Text> m_itemsRight;

  std::weak_ptr<Unit> m_hero;
  std::weak_ptr<Window> m_infoWindow;
  Text m_itemDescription;

  // Индекс части тела, на которую встанет выбранный в сумке предмет.
  int m_highlightedSlot = -1;

  // Печать Info-блока (описание выбранного предмета) внутри окна инвентаря.
  void renderItemInfo(const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd);

  void refresh();
  // Обновление только правой колонки (части тела) — вызывается при каждом show,
  // чтобы подсветка слота не «замораживалась» до событий equip/unequip.
  void refreshBodyParts();
};

#endif  // INVENTORY_WINDOW_H
