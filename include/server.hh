#ifndef NETCPP_SERVER_HH
#define NETCPP_SERVER_HH

#include "include/posix/descriptor.hh"
#include <memory>
#include <string>

#if defined(__linux__)
#include <sys/epoll.h>
#endif

class ServerHandler {
public:
  ServerHandler();
  virtual void onRequest() = 0;
};

class Server {
public:
  Server(const std::string address, const unsigned int port);
  inline void setHandler(std::unique_ptr<ServerHandler> handler) {
    handler_ = std::move(handler);
  }

  inline void run() {
#if defined(__linux__)
    runWithLinux();
#endif
  }

private:
  std::string address_;
  unsigned int port_;
  std::unique_ptr<ServerHandler> handler_;

#if defined(__linux__)
  int serverFd;
  int epollFd;
  epoll_event events[MAX_EVENT];

  void runWithLinux();
#endif
};

#endif // NETCPP_SERVER_HH
