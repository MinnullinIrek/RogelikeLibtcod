#ifndef IUNIT_H
#define IUNIT_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "../header.h"
#include "../unit_types.h"

class Bag;
class IMover;
class Chars;
class Interactor;
class BodyParts;

class IUnit : public ToChar {
 public:
  IUnit();
  IUnit(Identifier id, EUnitTypes uType = EUnitTypes::none);
  virtual ~IUnit();

  virtual Identifier toChar() const override;
  EUnitTypes m_type = EUnitTypes::none;

  void createChars();
  std::shared_ptr<Chars> getChars();
  void IUnit::setInteractor(std::shared_ptr<Interactor> interactor);
  std::shared_ptr<Interactor> getInteractor();
  void setBodyParts(std::shared_ptr<BodyParts> bp);
  std::shared_ptr<BodyParts> getBodyParts();

 protected:
  Identifier m_id;
  std::shared_ptr<Chars> m_chars;
  std::shared_ptr<Interactor> m_currentInteractor;
  std::shared_ptr<BodyParts> m_bodyParts;
};

class Unit : public IUnit, public ToString {
 public:
  Unit(const Identifier& id, std::shared_ptr<IMover> mover);
  Unit();
  Description toString() const override;
  virtual std::shared_ptr<IMover> getMover();
  virtual void setMover(std::shared_ptr<IMover> mover);
  void lookAround(bool isEyeOpened);
  const std::unordered_map<Coord, bool, KeyHasher>& getWatchingCoords() const;
  std::shared_ptr<Bag> getBag();

 protected:
  std::shared_ptr<Bag> m_bag;
  std::shared_ptr<IMover> m_mover;
  std::unordered_map<Coord, bool, KeyHasher> m_watchingCoords;
};

#endif
