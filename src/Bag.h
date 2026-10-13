#ifndef Bag_H
#define Bag_H

#include <memory>
#include <string>
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
  // Заменить выбранный предмет другим (например, вернуть снятую с тела вещь в сумку).
  void replaceSelected(std::shared_ptr<IItems> item);
  // Текущий выбранный предмет (или nullptr, если сумка пуста).
  std::shared_ptr<IItems> getSelected() const;
  void select(int next);
  std::list<Text> showBag(Count count);

 private:
  bool contains(std::shared_ptr<IItems> item);
  std::vector<std::shared_ptr<IItems>> m_items;
  int m_selected = -1;
};

#endif
