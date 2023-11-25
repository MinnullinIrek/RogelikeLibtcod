#ifndef IUNIT_H
#define IUNIT_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "../header.h"
#include "../unit_types.h"
#include "mover_interface.h"

class Bag;
class MoverInterface;
class Chars;
class Interactor;
class BodyParts;
class Effect;
// class Prototype;

class IUnit : public ToChar, public std::enable_shared_from_this<IUnit> {
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
  std::unique_ptr<Effect> getEffect();
  void acceptEffect(std::unique_ptr<Effect> effect);

 public:
  std::shared_ptr<Effect> m_effectProtoType;

 protected:
  Identifier m_id;
  std::shared_ptr<Chars> m_chars;
  std::shared_ptr<Interactor> m_currentInteractor;
  std::shared_ptr<BodyParts> m_bodyParts;
};

class Unit : public IUnit, public ToString, public MoverInterface /*decorator*/ {
 public:
  Unit(const Identifier& id, std::shared_ptr<MoverInterface> mover);
  Unit();
  Description toString() const override;
  virtual std::shared_ptr<MoverInterface> getMover();
  virtual void setMover(std::shared_ptr<MoverInterface> mover);
  void lookAround(bool isEyeOpened);
  const std::unordered_map<Coord, bool, KeyHasher>& getWatchingCoords() const;
  std::shared_ptr<Bag> getBag();

  void moveInDirection(const Coord& coord) override;
  void setCoord(const Coord& currentPos) override;
  const Coord& getCoord() const override;
  std::weak_ptr<Map> getMap() override;
  void changeMap(std::weak_ptr<Map> map) override;

 protected:
  std::shared_ptr<Bag> m_bag;
  std::shared_ptr<MoverInterface> m_mover;
  std::unordered_map<Coord, bool, KeyHasher> m_watchingCoords;
};

#endif
