#include "invoker.h"

#include "command.h"
const int undoCount = 4;
Invoker::Invoker() {}

Invoker::~Invoker() {}

void Invoker::setCommand(std::shared_ptr<Command> command) { m_commands.push(command); }

void Invoker::run() {
  if (!m_commands.empty()) {
    auto currentCommand = m_commands.back();
    m_commands.pop();
    currentCommand->execute();
    pushExecutedCommand(currentCommand);
  }
}

void Invoker::undo() {
  if (!m_executedCommands.empty()) {
    auto command = m_executedCommands.back();
    m_executedCommands.pop();
    command->undo();
  }
}

void Invoker::pushExecutedCommand(std::shared_ptr<Command> command) {
  shrinkQueue();
  m_executedCommands.push(command);
}

void Invoker::shrinkQueue() {
  while (m_executedCommands.size() > undoCount) {
    m_executedCommands.pop();
  }
}
