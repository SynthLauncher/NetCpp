#ifndef NETCPP_HTTP1_RESPONSE_HH
#define NETCPP_HTTP1_RESPONSE_HH

#include "include/utils/http_version.hh"
#include "include/utils/parse_headers.hh"
#include "include/utils/status.hh"
#include <string>
#include <unordered_map>
#include <vector>

class Http1Response {
public:
  Http1Response();
  Http1Response(std::string response);

  std::string url;
  HttpVersion version;
  Status status;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

#endif // NETCPP_HTTP1_RESPONSE_HH
