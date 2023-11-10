#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <memory>
#include <set>

class Actor;

class Keyboard {
 public:
  Keyboard(std::shared_ptr<Actor> actor);
  ~Keyboard();
  void setActor(std::shared_ptr<Actor> actor);
  void setKey(int key, bool down);
  void pushKeys(int key, bool down);
 protected:
  std::shared_ptr<Actor> m_actor;
  std::set<int> m_pressedKeys;
  int m_lastKeys = 0;
};
#endif
