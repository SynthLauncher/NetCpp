#include "include/http1/request.hh"

Http1Request::Http1Request() {}

Http1Request::Http1Request(std::string request) {
  std::vector<std::string> methods;
  std::string ss;

  int pos = request.find('\n');
  for (int i = 0; i < pos; ++i) {
    if (request[i] == ' ' || (request[i] == '\r' && request[i + 1] == '\n')) {
      methods.push_back(ss);
      ss.clear();

      if (request[i] == '\r') {
        break;
      }
    } else {
      ss.push_back(request[i]);
    }
  }

  method = stringToMethod(methods[0]);
  uri = methods[1];
  version = stringToHttpVersion(methods[2]);

  request.erase(request.begin(), request.begin() + pos + 1);

  pos = request.find("\r\n\r\n");

  headers =
      parseHeader(std::string(request.begin(), request.begin() + pos + 2));

  body = std::string(request.begin() + pos + 4, request.end());
}
