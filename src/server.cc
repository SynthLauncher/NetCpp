#include "include/server.hh"

Server::Server(const std::string address, const unsigned int port)
    : address_(address), port_{port} {}

#if defined(__linux__)
void Server::runWithLinux() {
  serverFd = createSocketServer(address_, port_);
  epollFd = createEpoll(serverFd);

  while (true) {
    int numEvent = epoll_wait(epollFd, events, MAX_EVENT, -1);

    for (int i = 0; i < numEvent; ++i) {
      if (events[i].data.fd == serverFd) {
        int client = accept(serverFd, NULL, NULL);
        setNonBlocking(client);

        std::unique_ptr<epoll_event> ev(new epoll_event);
        ev->data.fd = client;
        ev->events = EPOLLIN | EPOLLET;

        epoll_ctl(epollFd, EPOLL_CTL_ADD, client, ev.get());
      } else {
        char messageBuffer[MAX_BUFFER];

        int size = recv(events[i].data.fd, messageBuffer, MAX_BUFFER, 0);
        std::string message(messageBuffer, size);
      }
    }
  }
}
#endif
