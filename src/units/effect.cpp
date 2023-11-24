#include "effect.h"

#include "IUnit.h"
#include "chars.h"

Effect::~Effect() {}

TestEffect::~TestEffect() {}
void TestEffect::visit(std::weak_ptr<IUnit> unit) {
  auto locked = unit.lock();
  auto attacker = std::static_pointer_cast<Unit>(locked);
  auto chars = attacker->getChars();

  auto attackerStrength = chars->getValue(static_cast<int>(ECharTypes::strength));
  auto defenderChars = locked->getChars();
  auto defenderHp = defenderChars->getValue(static_cast<int>(ECharTypes::hp));
  defenderChars->setValue(static_cast<int>(ECharTypes::hp), defenderHp - attackerStrength);
}
