#ifndef NETCPP_HTTP2_REQUEST_HH
#define NETCPP_HTTP2_REQUEST_HH

#include "include/utils/http_version.hh"
#include "include/utils/method.hh"
#include "include/utils/parse_headers.hh"
#include <string>
#include <unordered_map>
#include <vector>

class Http2Request {
public:
  Http2Request();
  Http2Request(std::string request);

  void parseHeaders();
  void parseData();

  std::string rawRequest;
  std::string url;
  Method method;
  std::string uri;
  HttpVersion version;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
};

#endif // NETCPP_HTTP2_REQUEST_HH
