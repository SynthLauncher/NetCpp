#include "include/posix/descriptor.hh"

#if defined(__linux__)

int createSocketServer(const std::string &address, const unsigned int port) {
  int server = socket(AF_INET, SOCK_STREAM, 0);

  int opt = 1;
  if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(int)) != 0) {
    // TODO: HANDLE ERROR
  }

  std::unique_ptr<sockaddr_in> addr(new sockaddr_in);
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
  inet_aton(address.c_str(), &addr->sin_addr);

  if (bind(server, (sockaddr *)addr.get(), sizeof(sockaddr_in)) != 0) {
    // TODO: HANDLE ERROR
  }

  if (listen(server, 0) != 0) {
    // TODO: HANDLE ERROR
  }

  return server;
}

void setNonBlocking(int fd) {
  int flag = fcntl(fd, F_GETFL);
  fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

int createEpoll(int serverFd) {
  int epollfd = epoll_create1(0);
  std::unique_ptr<epoll_event> ev(new epoll_event);
  ev->data.fd = serverFd;
  ev->events = EPOLLIN | EPOLLET;

  epoll_ctl(epollfd, EPOLL_CTL_ADD, serverFd, ev.get());

  return epollfd;
}

#endif // OS Condition
