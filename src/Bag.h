#ifndef Bag_H
#define Bag_H

#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "header.h"
#include "utils/subscriber.h"

class IItems;

class Bag : public Publisher {
 public:
  Bag();
  ~Bag();
  void putItem(std::shared_ptr<IItems> item);
  void putItem(std::shared_ptr<Bag> bag);

  std::weak_ptr<IItems> popItem();
  void select(int next);
  /// Описание выбранного предмета: имя, текст описания и характеристики.
  using ItemDescription = std::map<std::string, std::string>;
  /// Возвращает пару: список предметов для отображения и описание выбранного предмета
  /// (пустая карта, если сумка пуста).
  std::pair<std::list<Text>, ItemDescription> showBag(Count count);

 private:
  bool contains(std::shared_ptr<IItems> item);
  std::vector<std::shared_ptr<IItems>> m_items;
  int m_selected = -1;
};

#endif
