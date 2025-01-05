#include "include/core/response.hh"
#include "utils/http_version.hh"
#include "utils/status.hh"
#include <string>
#include <vector>

HttpResponse::HttpResponse() {}

HttpResponse::HttpResponse(std::string response) {
  std::vector<std::string> methods;
  std::string ss;

  int pos = response.find('\n');
  for (int i = 0; i < pos; ++i) {
    if (response[i] == ' ' ||
        (response[i] == '\r' && response[i + 1] == '\n')) {
      methods.push_back(ss);
      ss.clear();

      if (response[i] == '\r') {
        break;
      }
    } else {
      ss.push_back(response[i]);
    }
  }

  version = stringToHttpVersion(methods[0]);
  status = intToStatus(std::stoi(methods[1]));

  response.erase(response.begin(), response.begin() + pos + 1);

  std::vector<std::string> header;
  ss.clear();

  pos = response.find("\r\n\r\n");

  for (int i = 0; i < pos + 2; ++i) {
    if (response[i] == ':' && response[i + 1] == ' ') {
      header.push_back(ss);
      ss.clear();
    } else if (response[i] == '\r' && response[i + 1] == '\n') {
      header.push_back(ss);
      ss.clear();

      headers[header[0]] = header[1];
      header.clear();
      ++i;
    } else if (response[i] == ' ' && response[i - 1] == ':') {
    } else {
      ss.push_back(response[i]);
    }
  }

  body = std::string(response.begin() + pos + 4, response.end());
}
