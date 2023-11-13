#include "time.h"

Time::Time() : m_currentTime(0.f) {}

Time::~Time() {}

void Time::notify(std::weak_ptr<Publisher> publisher) {}

void Time::addTime(float dt) {
  m_currentTime += dt;
  emit();
}
