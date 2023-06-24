#include "interactor.h"

#include <assert.h>
#include <stdio.h>

#include "../maps/cell.h"
#include "../unit_types.h"
#include "../units/IUnit.h"

Interactor::Interactor(/* args */) {}

Interactor::~Interactor() {}

void Interactor::interact(std::shared_ptr<Cell> cell1, std::shared_ptr<Cell> cell2) {
  auto unit1 = cell1->getUnit();
  auto unit2 = cell2->getUnit();
  assert(unit1);

  if (unit2) {
    if (unit2->m_type == EUnitTypes::wall) {
      return;
    } else {
      attack(unit1, unit2);
    }

  } else {
    cell2->setUnit(unit1);
  }
}

void Interactor::attack(std::shared_ptr<IUnit> unit1, std::shared_ptr<IUnit> unit2) { printf("attack not ready"); }
