#include "../../include/core/request.hh"
#include <vector>

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(std::string request) {
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

  std::vector<std::string> header;
  ss.clear();

  pos = request.find("\r\n\r\n");

  for (int i = 0; i < pos + 2; ++i) {
    if (request[i] == ':' && request[i + 1] == ' ') {
      header.push_back(ss);
      ss.clear();
    } else if (request[i] == '\r' && request[i + 1] == '\n') {
      header.push_back(ss);
      ss.clear();

     headers[header[0]] = header[1];
      header.clear();
      ++i;
    } else if (request[i] == ' ' && request[i - 1] == ':') {
    } else {
      ss.push_back(request[i]);
    }
  }

  body = std::string(request.begin() + pos + 4, request.end());
}
