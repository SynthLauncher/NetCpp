#ifndef NETCPP_SERVER_HH
#define NETCPP_SERVER_HH

#include "include/posix/descriptor.hh"
#include <string>

#if defined(__linux__)
#include <sys/epoll.h>
#endif

class Server {
public:
  Server(const std::string address, const unsigned int port);
  
  inline void run() {
    #if defined(__linux__)
        runWithLinux();
    #endif
  }

private:
  std::string address_;
  unsigned int port_;

  #if defined(__linux__)
    int serverFd;
    int epollFd;
    epoll_event events[MAX_EVENT];

    void runWithLinux();
  #endif
};

#endif // NETCPP_SERVER_HH
