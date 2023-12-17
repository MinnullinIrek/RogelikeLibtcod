#ifndef IUNIT_H
#define IUNIT_H

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../char_types.h"
#include "../header.h"
#include "../unit_types.h"
#include "mover_interface.h"

using CharSubscriber = std::function<void(CharValueType)>;

class Bag;
class MoverInterface;
class Chars;
class Interactor;
class BodyParts;
class Effect;
class CorpsDeleter;
// class Prototype;

class IUnit : public ToChar, public MoverInterface /*decorator*/ {
 public:
  IUnit();
  virtual EUnitTypes getType() = 0;
  virtual ~IUnit();

  // virtual Identifier toChar() const override;

  virtual void createChars() = 0;
  virtual std::shared_ptr<Chars> getChars() = 0;
  virtual void setInteractor(std::shared_ptr<Interactor> interactor) = 0;
  virtual std::shared_ptr<Interactor> getInteractor() = 0;
  virtual void setBodyParts(std::shared_ptr<BodyParts> bp) = 0;
  virtual std::shared_ptr<BodyParts> getBodyParts() = 0;
  virtual std::unique_ptr<Effect> getEffect() = 0;
  virtual void acceptEffect(std::unique_ptr<Effect> effect) = 0;
  virtual void die() = 0;
};

class Unit : public IUnit, public ToString, public Subscriber {
 public:
  Unit(const Identifier& id, std::shared_ptr<MoverInterface> mover, EUnitTypes type = EUnitTypes::enemy);
  Unit();
  ~Unit();
  EUnitTypes getType() override;

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
  virtual Identifier toChar() const override;
  void acceptEffect(std::unique_ptr<Effect> effect) override;
  std::unique_ptr<Effect> getEffect() override;
  void createChars();
  std::shared_ptr<Chars> getChars();
  void setBodyParts(std::shared_ptr<BodyParts> bp);
  std::shared_ptr<BodyParts> getBodyParts();
  void createTestEffect();
  void setInteractor(std::shared_ptr<Interactor> interactor) override;
  std::shared_ptr<Interactor> getInteractor() override;
  void die() override;
  //--Subscriber
  void notify(std::weak_ptr<Publisher> publisher) override;
  void setCharSubscriber(int charType, CharSubscriber&& charSub);
  void setCorpsDeleter(std::shared_ptr<CorpsDeleter> deleter);

 public:
  std::shared_ptr<Effect> m_effectProtoType;
  EUnitTypes m_type = EUnitTypes::none;

 protected:
  Identifier m_id;
  std::shared_ptr<Bag> m_bag;
  std::shared_ptr<MoverInterface> m_mover;
  std::unordered_map<Coord, bool, KeyHasher> m_watchingCoords;
  std::shared_ptr<Chars> m_chars;
  std::shared_ptr<BodyParts> m_bodyParts;
  std::shared_ptr<Interactor> m_currentInteractor;
  std::unordered_map<int, CharSubscriber> m_charSubscribers;
  std::weak_ptr<CorpsDeleter> m_corpsDeleter;
};

#endif
