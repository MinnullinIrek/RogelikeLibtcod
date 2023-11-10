#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <functional>
#include <list>
#include <memory>
#include <unordered_map>

class Connections;
class Subscriber;

using PublisherKey = unsigned int;
using SubscriberKey = std::pair<PublisherKey, std::weak_ptr<Subscriber>>;

class Publisher {
 public:
  virtual ~Publisher(){};

  // todo to private friend Connection
  void setConnection(std::weak_ptr<Connections> connection, PublisherKey publisherNum);
  PublisherKey getPublisherKey();  // todo to private friend Connection
  void emit();

 protected:
 private:
  std::weak_ptr<Connections> m_connection;
  PublisherKey m_publisherKey = 0;
};

class Subscriber {
 public:
  virtual ~Subscriber(){};
  virtual void notify(std::weak_ptr<Publisher> publisher) = 0;
};

class Connections {
 public:
  // static Connections& instance() {
  //   static Connections connection;
  //   return connection;
  // }
  // ~Connections();
  PublisherKey registerPublisher(std::weak_ptr<Publisher> publisher);
  SubscriberKey addSubscriber(std::shared_ptr<Publisher> publisher, std::shared_ptr<Subscriber> subscriber);
  void removeSubscriber(SubscriberKey&& key);
  void notifySubscribers(PublisherKey publisherNum);
  // Connections();

 private:
  std::unordered_map<PublisherKey, std::list<std::weak_ptr<Subscriber>>> m_subscribers;
  std::unordered_map<PublisherKey, std::weak_ptr<Publisher>> m_publishers;

  PublisherKey m_publisherCount = 0;
};

class Connector {
 public:
  static Connector& instance() {
    static Connector connection;
    return connection;
  }

  void connect(std::shared_ptr<Publisher> publisher, std::shared_ptr<Subscriber> subscriber);
  void unsubscribe(std::shared_ptr<Publisher> publisher, std::shared_ptr<Subscriber> subscriber);

 private:
  Connector();
  std::shared_ptr<Connections> m_connections;
};

#endif  // SUBSCRIBER_H
