#ifndef INFO_H
#define INFO_H

#include <memory>
#include <string>

class Unit;

class Info {
 public:
  Info(/* args */);
  ~Info();
  void setHero(std::weak_ptr<Unit> hero);
  std::string getText();

 private:
  std::weak_ptr<Unit> m_mainHero;
};

#endif
