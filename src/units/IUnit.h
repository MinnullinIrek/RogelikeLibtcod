#ifndef IUNIT_H
#define IUNIT_H

#include <memory>
#include <unordered_map>

#include "../header.h"

class Bag;

class IUnit : public ToChar {
 public:
  IUnit();
  IUnit(Identifier id);

  virtual Identifier toChar() const override;

 protected:
  Identifier m_id;
};

class Unit : public IUnit, public ToString {
 public:
  Unit(const Identifier& id);
  Unit();
  // Identifier toChar() const override;
  Description toString() override;

 protected:
  std::unordered_map<ESettings, Setting> m_settings;
  UnitType m_type;
  // std::unique_ptr<Bag> m_bag;
};

#endif
