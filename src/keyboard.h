#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <memory>
//#include <functional>
//#include <unordered_map>

class Actor;

class Keyboard {
 public:
  Keyboard(std::shared_ptr<Actor> actor);
  ~Keyboard();
  void setActor(std::shared_ptr<Actor> actor);
  void setKey(int key);

 protected:
  std::shared_ptr<Actor> m_actor;
  //std::unordered_map<int, std::function<void(int)>> m_actions;
};
#endif
