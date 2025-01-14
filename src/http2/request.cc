#include "include/http2/request.hh"
#include "utils/parse_headers.hh"

Http2Request::Http2Request() {}

Http2Request::Http2Request(std::string request) : rawRequest{request} {}

void Http2Request::parseHeaders() { headers = parseHeader(rawRequest); }
