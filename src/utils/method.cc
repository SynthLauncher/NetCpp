#include "include/utils/method.hh"

std::string methodToString(Method method) {
  switch (method) {
  case Method::GET:
    return "GET";
  case Method::HEAD:
    return "HEAD";
  case Method::OPTIONS:
    return "OPTIONS";
  case Method::TRACE:
    return "TRACE";
  case Method::PUT:
    return "PUT";
  case Method::DELETE:
    return "DELETE";
  case Method::POST:
    return "POST";
  case Method::PATCH:
    return "PATCH";
  case Method::CONNECT:
    return "CONNECT";
  default:
    // TODO: HANDLE ERROR
    return "GET";
  }
}

Method stringToMethod(std::string method) {
  if (method == "GET") {
    return Method::GET;
  } else if (method == "HEAD") {
    return Method::HEAD;
  } else if (method == "OPTIONS") {
    return Method::OPTIONS;
  } else if (method == "TRACE") {
    return Method::TRACE;
  } else if (method == "PUT") {
    return Method::PUT;
  } else if (method == "DELETE") {
    return Method::DELETE;
  } else if (method == "POST") {
    return Method::POST;
  } else if (method == "PATCH") {
    return Method::PATCH;
  } else if (method == "CONNECT") {
    return Method::CONNECT;
  } else {
    // TODO: HANDLE ERROR
    return Method::GET;
  }
}
