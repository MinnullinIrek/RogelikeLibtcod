#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <memory>

#include "asio.hpp"
#include "serialize_buffer.h"

class Channel_impl;
// class Serializable;

class Channel final {
 public:
  Channel(int port = 2001, const std::string& ip = "127.0.0.1");
  void send(std::shared_ptr<Serializable> object, const std::function<void(int)>& err_func = nullptr);
  SerializeBuffer read();
  ~Channel();

 private:
  std::unique_ptr<Channel_impl> impl;
};

#endif  // CHANNEL_H
