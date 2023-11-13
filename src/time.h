#ifndef TIME_H
#define TIME_H

#include "utils/subscriber.h"

class Time : public Subscriber, public Publisher {
 public:
  Time();
  ~Time();
  void notify(std::weak_ptr<Publisher> publisher);
  void addTime(float dt);

 private:
  float m_currentTime;
};

#endif
