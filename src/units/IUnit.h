#ifndef IUNIT_H
#define IUNIT_H

#include <memory>
#include <unordered_map>

#include "../header.h"
#include "../unit_types.h"

class Bag;
class IMover;
class Chars;

class IUnit : public ToChar {
 public:
  IUnit();
  IUnit(Identifier id);

  virtual Identifier toChar() const override;
  EUnitTypes m_type;

 protected:
  Identifier m_id;
};

class Unit : public IUnit, public ToString {
 public:
  Unit(const Identifier& id, std::shared_ptr<IMover> mover);
  Unit();
  Description toString() override;
  virtual std::shared_ptr<IMover> getMover();
  virtual void setMover(std::shared_ptr<IMover> mover);

 protected:
  std::unordered_map<ESettings, Setting> m_settings;
  UnitType m_type;
  // std::unique_ptr<Bag> m_bag;
  std::shared_ptr<IMover> m_mover;
};

#endif
