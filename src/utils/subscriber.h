#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <functional>
#include <list>
#include <memory>
#include <unordered_map>

class Subscriber;

using PublisherKey = unsigned int;
using SubscriberKey = unsigned int;

class Publisher : public std::enable_shared_from_this<Publisher> {
 public:
  virtual ~Publisher(){};

  // todo to private friend Connection
  void emit();
  void addSubscriber(std::weak_ptr<Subscriber> subscriber);
  void removeSubscriber(std::weak_ptr<Subscriber> subscriber);

 private:
  void checkZompies();
  PublisherKey m_publisherKey = 0;
  std::unordered_map<SubscriberKey, std::weak_ptr<Subscriber>> m_subscribers;
};

class Subscriber {
 public:
  Subscriber();
  virtual ~Subscriber();
  virtual void notify(std::weak_ptr<Publisher> publisher) = 0;

 private:
  struct Impl;
  SubscriberKey getKey();
  friend Publisher;
  Impl* m_impl;
};



#endif  // SUBSCRIBER_H
