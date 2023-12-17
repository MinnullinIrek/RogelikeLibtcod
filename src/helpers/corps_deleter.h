#ifndef CORPS_DELETER_H
#define CORPS_DELETER_H
#include <memory>

class IUnit;

class CorpsDeleter {
 public:
  CorpsDeleter() = default;
  virtual ~CorpsDeleter() = default;
  void removeCorps(std::weak_ptr<IUnit> unit);

 private:
};

#endif
