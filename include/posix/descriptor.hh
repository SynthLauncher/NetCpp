#ifndef NETCPP_POSIX_DESCRIPTOR_HH
#define NETCPP_POSIX_DESCRIPTOR_HH

#include <memory>
#include <string>

#if defined(__linux__)
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define MAX_EVENT 15
#define MAX_BUFFER 2048

int createSocketServer(const std::string &address, const unsigned int port);

void setNonBlocking(int fd);

int createEpoll(int serverFd);

#endif // OS Condition

#endif // NETCPP_POSIX_DESCRIPTOR_HH
