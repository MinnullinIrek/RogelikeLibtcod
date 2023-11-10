#ifndef COMMAND_H
#define COMMAND_H

#include <functional>

class Command {
 public:
  Command(std::function<void()>&& executeFunc, std::function<void()>&& undoFunc = nullptr);
  virtual void execute();
  virtual void undo();
  std::function<void()> m_executeFunc;
  std::function<void()> m_undoFunc;
};

#endif  // COMMAND_H
