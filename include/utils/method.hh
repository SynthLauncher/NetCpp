#ifndef NETCPP_UTILS_METHOD_HH
#define NETCPP_UTILS_METHOD_HH

#include <string>

enum class Method {
  GET,
  HEAD,
  OPTIONS,
  TRACE,
  PUT,
  DELETE,
  POST,
  PATCH,
  CONNECT,
};

std::string methodToString(Method method);

Method stringToMethod(std::string method);

#endif // NETCPP_UTILS_METHOD_HH
