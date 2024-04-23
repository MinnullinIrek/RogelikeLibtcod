#include "channel.h"

#include "../game_struct.h"
#include "../header.h"

class Channel_impl {
 public:
  ~Channel_impl() = default;
  Channel_impl(int port = 2001, const std::string& ip = "127.0.0.1")
      : ep(asio::ip::address::from_string(ip), port), acc(service, ep), socket_ptr(new asio::ip::tcp::socket(service)) {
//    acc.accept(*socket_ptr);
  }

  int send(const std::vector<std::byte> buffer, const std::function<void(int)>& err_func = nullptr) {
    return write(*socket_ptr, asio::buffer(buffer, buffer.size()));
  }

  size_t read(std::vector<std::byte>& buffer) {
    auto count = socket_ptr->read_some(asio::buffer(buffer));
    return count;
  }

 private:
  asio::io_service service;

  asio::ip::tcp::endpoint ep;
  asio::ip::tcp::acceptor acc;
  std::unique_ptr<asio::ip::tcp::socket> socket_ptr;
};

Channel ::~Channel() {}

Channel::Channel(int port, const std::string& ip) : impl(new Channel_impl) {}

void Channel::send(std::shared_ptr<Serializable> object, const std::function<void(int)>& err_func) {
  return;
  auto buffer = object->serialize();
  impl->send(buffer.getBuf());
}

SerializeBuffer Channel::read() {
  SerializeBuffer buf;
  impl->read(buf.getBuf());
  return buf;
}
