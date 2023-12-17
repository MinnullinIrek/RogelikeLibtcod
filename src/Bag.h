#ifndef Bag_H
#define Bag_H

#include <memory>
#include <string>
#include <unordered_map>

#include "header.h"
#include "utils/subscriber.h"

struct Item_Count;
class IItems;

class Bag : public Publisher {
 public:
  Bag();
  ~Bag();
  void putItem(std::shared_ptr<IItems> item, Count count);
  void putItem(std::shared_ptr<Bag> bag);

  std::pair<std::weak_ptr<IItems>, Count> popItem(Count popCount);
  void select(int next);
  std::list<Text> showBag(Count count);

 private:
  void putInsert(std::shared_ptr<IItems> item, Count count);
  bool contains(std::shared_ptr<IItems> item);
  std::unordered_map<std::shared_ptr<IItems>, Count> m_items;
  std::unordered_map<std::shared_ptr<IItems>, Count>::iterator m_selected;
};

#endif
