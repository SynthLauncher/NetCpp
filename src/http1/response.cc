#include "include/http1/response.hh"

Http1Response::Http1Response() {}

Http1Response::Http1Response(std::string response) {
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

  pos = response.find("\r\n\r\n");

  headers =
      parseHeader(std::string(response.begin(), response.begin() + pos + 2));

  body = std::string(response.begin() + pos + 4, response.end());
}
