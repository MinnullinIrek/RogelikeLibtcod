#include "subscriber.h"

PublisherKey Publisher::getPublisherKey() { return m_publisherKey; }
void Publisher::setConnection(std::weak_ptr<Connections> connection, PublisherKey publisherNum) {
  m_connection = connection;
  m_publisherKey = publisherNum;
}

void Publisher::emit() {
  auto connection = m_connection.lock();

  if (connection) {
    connection->notifySubscribers(m_publisherKey);
  }
}

PublisherKey Connections::registerPublisher(std::weak_ptr<Publisher> publisher) {
  auto pub = publisher.lock();
  if (pub) {
    if (m_publishers.find(pub->getPublisherKey()) == m_publishers.end()) {
      ++m_publisherCount;
      m_publishers[m_publisherCount] = publisher;
    } else {
      return pub->getPublisherKey();
    }
  }
  return m_publisherCount;
}

void Connections::notifySubscribers(PublisherKey publisherNum) {
  auto publisher = m_publishers.at(publisherNum).lock();
  if (publisher) {
    auto subscribers = m_subscribers.at(publisherNum);
    auto itSub = subscribers.begin();
    for (; itSub != subscribers.end();) {
      auto lockedSub = itSub->lock();
      if (lockedSub) {
        lockedSub->notify(publisher);
        ++itSub;
      } else {
        subscribers.erase(itSub);
      }
    }
  } else {
    m_publishers.erase(publisherNum);
    m_subscribers.erase(publisherNum);
  }
}

SubscriberKey Connections::addSubscriber(std::shared_ptr<Publisher> publisher, std::shared_ptr<Subscriber> subscriber) {
  auto publisherKey = publisher->getPublisherKey();
  m_subscribers[publisherKey].push_back(subscriber);
  //}
  return {publisherKey, subscriber};
}

void Connections::removeSubscriber(SubscriberKey&& key) {
  auto subscribersIt = m_subscribers.find(key.first);

  if (subscribersIt != m_subscribers.end()) {
    auto lockedRemoing = key.second.lock();
    // subscribersIt->second.remove(key.second);
    auto subIt = subscribersIt->second.begin();
    for (; subIt != subscribersIt->second.end();) {
      auto subscriber = subIt->lock();
      if (!subscriber) {
        subIt = subscribersIt->second.erase(subIt);
      } else if (subscriber == lockedRemoing) {
        subscribersIt->second.erase(subIt);
        break;
      } else {
        ++subIt;
      }
    }
  }

  if (subscribersIt->second.size() == 0) {
    m_subscribers.erase(key.first);
    m_publishers.erase(key.first);
  }
}

void Connector::connect(std::shared_ptr<Publisher> publisher, std::shared_ptr<Subscriber> subscriber) {
  auto publisherKey = m_connections->registerPublisher(publisher);
  publisher->setConnection(m_connections, publisherKey);
  m_connections->addSubscriber(publisher, subscriber);
}

void Connector::unsubscribe(std::shared_ptr<Publisher> publisher, std::shared_ptr<Subscriber> subscriber) {
  m_connections->removeSubscriber({publisher->getPublisherKey(), std::weak_ptr<Subscriber>(subscriber)});
};

Connector::Connector() : m_connections(std::make_shared<Connections>()) {}
