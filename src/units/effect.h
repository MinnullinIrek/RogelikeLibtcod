#ifndef EFFECT_H
#define EFFECT_H

#include <memory>

#include "../utils/prototype.h"

class IUnit;

class Effect
  //: public Prototype
{
 public:
  virtual ~Effect();
  virtual void visit(std::weak_ptr<IUnit> defender) = 0;
  virtual std::unique_ptr<Effect> clone() = 0;
};

class TestEffect : public Effect {
 public:
  std::weak_ptr<IUnit> m_attacker;
  virtual ~TestEffect();
  void visit(std::weak_ptr<IUnit> unit) override;
  std::unique_ptr<Effect> clone() override {
    auto effect = std::make_unique<TestEffect>();
    effect->m_attacker = m_attacker;
    return effect;
  }
  //void clone() override{};
};

#endif
