#ifndef IUNIT_H
#define IUNIT_H

#include <memory>
#include <unordered_map>

#include "../header.h"

class Bag;

class IUnit {
 public:
  IUnit();
  Identifier m_id;
};

//class Unit : public IUnit, public ToChar, public ToString {
// public:
//  // Unit(const Identifier& id);
//  // Unit();
//  Identifier toChar() override;
//  Description toString() override;
//
// protected:
//  Identifier m_id;
//  std::unordered_map<ESettings, Setting> m_settings;
//  UnitType m_type;
//  std::unique_ptr<Bag> m_bag;
//};

#endif
