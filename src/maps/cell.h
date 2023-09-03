#ifndef CELL_H
#define CELL_H

#include <memory>

#include "../header.h"

class Bag;
class IUnit;

class Cell : public ToChar {
 public:
  Cell();
  ~Cell();
  Identifier toChar() const override;
  void setUnit(std::shared_ptr<IUnit> unit);
  std::shared_ptr<IUnit> getUnit();
  std::shared_ptr<Bag> getBag();
  bool isWall() const;
  void setSeen(bool isSeen);

 protected:
  Coord m_coord;
  std::shared_ptr<Bag> m_bag;
  std::shared_ptr<IUnit> m_unit;
  Identifier m_id;
  bool m_isSeen = false;
};

#endif
