#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <memory>
class IUnit;
class Cell;

class Interactor {
 private:
  /* data */
 public:
  Interactor(/* args */);
  ~Interactor();
  virtual void interact(std::shared_ptr<IUnit> unit1, std::shared_ptr<IUnit> unit2);
  virtual void attack(std::shared_ptr<IUnit> unit1, std::shared_ptr<IUnit> unit2);
};

#endif
