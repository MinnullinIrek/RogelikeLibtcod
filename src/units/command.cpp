#include "command.h"

Command::Command(std::function<void()>&& executeFunc, std::function<void()>&& undoFunc)
    : m_executeFunc(executeFunc), m_undoFunc(undoFunc) {}

void Command::execute() {
  if (m_executeFunc) {
    m_executeFunc();
  }
}

void Command::undo() {
  if (m_undoFunc) {
    m_undoFunc();
  }
}
std::function<void()> m_executeFunc;
std::function<void()> m_undoFunc;
