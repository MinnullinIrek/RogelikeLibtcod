#include "channel.h"

class Channel_impl {
 public:
  Channel_impl(int port, const std::string& ip = "127.0.0.1");
  void send(char* buffer, int count, const std::function<void(int)>& err_func = nullptr);

 private:
  asio::io_service service;

  asio::ip::tcp::endpoint ep;
  asio::ip::tcp::acceptor acc;
  std::unique_ptr<asio::ip::tcp::socket> socket_ptr;
};
