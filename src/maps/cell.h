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
  void setUnit(std::unique_ptr<IUnit> unit);

 protected:
  Coord m_coord;
  // std::unique_ptr<Bag> m_bag;
  std::unique_ptr<IUnit> m_unit;
  Identifier m_id;
};

#endif
