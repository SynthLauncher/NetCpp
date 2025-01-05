#include "include/utils/parse_headers.hh"
#include <vector>

std::unordered_map<std::string, std::string> parseHeader(std::string headers) {
  std::unordered_map<std::string, std::string> result;
  std::vector<std::string> header;
  std::string ss;

  for (size_t i = 0; i < headers.size(); ++i) {
    if (headers[i] == ':' && headers[i + 1] == ' ') {
      header.push_back(ss);
      ss.clear();
    } else if (headers[i] == '\r' && headers[i + 1] == '\n') {
      header.push_back(ss);
      ss.clear();

      result[header[0]] = header[1];
      header.clear();
      ++i;
    } else if (headers[i] == ' ' && headers[i - 1] == ':') {
    } else {
      ss.push_back(headers[i]);
    }
  }

  return result;
}
