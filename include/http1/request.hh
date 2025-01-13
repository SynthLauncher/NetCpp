#ifndef NETCPP_HTTP1_REQUEST_HH
#define NETCPP_HTTP1_REQUEST_HH

#include "include/utils/http_version.hh"
#include "include/utils/method.hh"
#include "include/utils/parse_headers.hh"
#include <string>
#include <unordered_map>
#include <vector>

class Http1Request {
public:
  Http1Request();
  Http1Request(std::string request);

  std::string url;
  Method method;
  std::string uri;
  HttpVersion version;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

#endif // NETCPP_HTTP1_REQUEST_HH
