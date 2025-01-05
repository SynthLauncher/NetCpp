#ifndef NETCPP_CORE_RESPONSE_HH
#define NETCPP_CORE_RESPONSE_HH

#include "include/utils/http_version.hh"
#include "include/utils/status.hh"
#include <string>
#include <unordered_map>

#define POWERED_BY "NetCpp/1.0.0"

class HttpResponse {
public:
  HttpResponse();
  HttpResponse(std::string response);

  std::string url;
  HttpVersion version;
  Status status;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

#endif // NETCPP_CORE_RESPONSE_HH
