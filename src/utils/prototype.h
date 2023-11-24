#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <memory>

class Prototype {
 public:
  virtual ~Prototype() {}
  virtual std::unique_ptr<Prototype> clone() = 0;
  //virtual void clone() = 0;
};

#endif
