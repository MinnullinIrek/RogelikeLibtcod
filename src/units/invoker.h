#ifndef INVOKER_H
#define INVOKER_H

#include <memory>
#include <queue>

class Command;

class Invoker {
 public:
  Invoker();
  ~Invoker();
  void setCommand(std::shared_ptr<Command> command);
  void run();
  void undo();

 private:
  void pushExecutedCommand(std::shared_ptr<Command> command);
  void shrinkQueue();
  std::queue<std::shared_ptr<Command>> m_commands;
  std::queue<std::shared_ptr<Command>> m_executedCommands;
};

#endif  // INVOKER_H
