#include "subscriber.h"

#include <algorithm>


struct Subscriber::Impl {
  Impl() : key(count) { ++count; }
  static unsigned int count;  // todo check overflow
  const unsigned int key;
};

unsigned int Subscriber::Impl::count = 0;
Subscriber::~Subscriber() { delete m_impl; }
Subscriber::Subscriber() : m_impl(new Impl) {}

void Publisher::emit() {
  std::weak_ptr wThis = shared_from_this();
  std::for_each(m_subscribers.begin(), m_subscribers.end(), [wThis](auto& subscriber) {
    auto lockedSubscriber = subscriber.second.lock();
    if (lockedSubscriber) {
      lockedSubscriber->notify(wThis);
    }
  });
}

SubscriberKey Subscriber::getKey() { return m_impl->key; }

void Publisher::addSubscriber(std::weak_ptr<Subscriber> subscriber) {
  auto locked = subscriber.lock();
  if (locked) {
    m_subscribers[locked->getKey()] = subscriber;
  }
}

void Publisher::removeSubscriber(std::weak_ptr<Subscriber> subscriber) {
  auto locked = subscriber.lock();
  if (locked) {
    m_subscribers.erase(locked->getKey());
  } else {
    checkZompies();
  }
}

void Publisher::checkZompies() {
  auto it = m_subscribers.begin();
  for (; it != m_subscribers.end();) {
    auto locked = it->second.lock();
    if (!locked) {
      it = m_subscribers.erase(it);
    }
    ++it;
  }
}
