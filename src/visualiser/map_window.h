#ifndef MAP_WINDOW_H
#define MAP_WINDOW_H

#include <unordered_map>

#include "../header.h"
#include "../utils/subscriber.h"
#include "IdentefierContainer.h"
#include "iwindow.h"

class MapWindow : public IWindow, public Subscriber {
 public:
  const std::string name = "mapwindow";
  MapWindow(const Rectangle& r);
  virtual void show(
      const std::function<void(Text&&, const Coord&)>& visualizator, const Coord& parentCd) const override;

  void notify(std::weak_ptr<Publisher> publisher) override;

 private:
  std::unordered_map<Coord, Identifier, KeyHasher> m_cells;
  IdentifierContainer<ConsoleIdentifier> m_idContainer;
};

#endif  // MAP_WINDOW_H
