#ifndef NETCPP_CORE_REQUEST_HH
#define NETCPP_CORE_REQUEST_HH

#include "include/utils/http_version.hh"
#include "include/utils/method.hh"
#include <string>
#include <unordered_map>

#define USER_AGENT "NetCpp/1.0.0"

class HttpRequest {
public:
  HttpRequest();
  HttpRequest(std::string request);

  std::string url;
  Method method;
  std::string uri;
  HttpVersion version;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

#endif // NETCPP_CORE_REQUEST_HH
