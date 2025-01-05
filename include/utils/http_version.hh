#ifndef NETCPP_UTILS_HTTP_VERSION_HH
#define NETCPP_UTILS_HTTP_VERSION_HH

#include <string>

enum class HttpVersion {
  HTTP1,
  HTTP2,
  HTTP3,
};

std::string httpVersionToString(HttpVersion version);

HttpVersion stringToHttpVersion(std::string version);

#endif // NETCPP_UTILS_HTTP_VERSION_HH
