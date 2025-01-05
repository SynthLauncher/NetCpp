#include "include/utils/status.hh"

Status intToStatus(int status) {
  if (status == 100) {
    return Status::ContinueStatus;
  } else if (status == 101) {
    return Status::SwitchingProtocols;
  } else if (status == 102) {
    return Status::Processing;
  } else if (status == 103) {
    return Status::EarlyHints;
  } else if (status == 200) {
    return Status::Ok;
  } else if (status == 201) {
    return Status::Created;
  } else if (status == 202) {
    return Status::Accepted;
  } else if (status == 203) {
    return Status::NonAuthoritative;
  } else if (status == 204) {
    return Status::NoContent;
  } else if (status == 205) {
    return Status::ResetContent;
  } else if (status == 206) {
    return Status::PartialContent;
  } else if (status == 207) {
    return Status::MultiStatus;
  } else if (status == 208) {
    return Status::AlreadyReported;
  } else if (status == 226) {
    return Status::ImUsed;
  } else if (status == 300) {
    return Status::MultipleChoices;
  } else if (status == 301) {
    return Status::MovedPermanently;
  } else if (status == 302) {
    return Status::Found;
  } else if (status == 303) {
    return Status::SeeOther;
  } else if (status == 304) {
    return Status::NotModified;
  } else if (status == 305) {
    return Status::UseProxy;
  } else if (status == 306) {
    return Status::SwitchProxy;
  } else if (status == 307) {
    return Status::TemporaryRedirect;
  } else if (status == 308) {
    return Status::PermanentRedirect;
  } else if (status == 400) {
    return Status::BadRequest;
  } else if (status == 401) {
    return Status::Unauthorized;
  } else if (status == 402) {
    return Status::PaymentRequired;
  } else if (status == 403) {
    return Status::Forbidden;
  } else if (status == 404) {
    return Status::NotFound;
  } else if (status == 405) {
    return Status::MethodNotAllowed;
  } else if (status == 406) {
    return Status::NotAcceptable;
  } else if (status == 407) {
    return Status::ProxyAuthenticationRequired;
  } else if (status == 408) {
    return Status::RequestTimeout;
  } else if (status == 409) {
    return Status::Conflict;
  } else if (status == 410) {
    return Status::Gone;
  } else if (status == 411) {
    return Status::LengthRequired;
  } else if (status == 412) {
    return Status::PreconditionFailed;
  } else if (status == 413) {
    return Status::PayloadTooLarge;
  } else if (status == 414) {
    return Status::UriTooLong;
  } else if (status == 415) {
    return Status::UnsupportedMediaType;
  } else if (status == 416) {
    return Status::RangeNotSatisfiable;
  } else if (status == 417) {
    return Status::ExpectationFailed;
  } else if (status == 418) {
    return Status::ImATeapot;
  } else if (status == 421) {
    return Status::MisdirectedRequest;
  } else if (status == 422) {
    return Status::UnprocessableContent;
  } else if (status == 423) {
    return Status::Locked;
  } else if (status == 424) {
    return Status::FailedDependency;
  } else if (status == 425) {
    return Status::TooEarly;
  } else if (status == 426) {
    return Status::UpgradeRequired;
  } else if (status == 428) {
    return Status::PreconditionRequired;
  } else if (status == 429) {
    return Status::TooManyRequests;
  } else if (status == 431) {
    return Status::RequestHeaderFieldsTooLarge;
  } else if (status == 451) {
    return Status::UnavailableForLegalReasons;
  } else if (status == 500) {
    return Status::InternalServerError;
  } else if (status == 501) {
    return Status::NotImplemented;
  } else if (status == 502) {
    return Status::BadGateway;
  } else if (status == 503) {
    return Status::ServiceUnavailable;
  } else if (status == 504) {
    return Status::GatewayTimeout;
  } else if (status == 505) {
    return Status::HttpVersionNotSupported;
  } else if (status == 506) {
    return Status::VariantAlsoNegotiates;
  } else if (status == 507) {
    return Status::InsufficientStorage;
  } else if (status == 508) {
    return Status::LoopDetected;
  } else if (status == 510) {
    return Status::NotExtended;
  } else if (status == 511) {
    return Status::NetworkAuthenticationRequired;
  } else {
    // TODO: HANDLE ERROR
    return Status::NotImplemented;
  }
}

std::string statusToString(Status status) {
  switch (status) {
  case Status::ContinueStatus:
    return "100 Continue";

  case Status::SwitchingProtocols:
    return "101 Switching Protocols";

  case Status::Processing:
    return "102 Processing";

  case Status::EarlyHints:
    return "103 Early Hints";

  case Status::Ok:
    return "200 OK";

  case Status::Created:
    return "201 Created";

  case Status::Accepted:
    return "202 Accepted";

  case Status::NonAuthoritative:
    return "203 Non-Authoritative Information";

  case Status::NoContent:
    return "204 No Content";

  case Status::ResetContent:
    return "205 Reset Content";

  case Status::PartialContent:
    return "206 Partial Content";

  case Status::MultiStatus:
    return "207 Multi-Status";

  case Status::AlreadyReported:
    return "208 Already Reported";

  case Status::ImUsed:
    return "226 IM Used";

  case Status::MultipleChoices:
    return "300 Multiple Choices";

  case Status::MovedPermanently:
    return "301 Moved Permanently";

  case Status::Found:
    return "302 Found";

  case Status::SeeOther:
    return "303 See Other";

  case Status::NotModified:
    return "304 Not Modified";

  case Status::UseProxy:
    return "305 Use Proxy";

  case Status::SwitchProxy:
    return "306 Switch Proxy";

  case Status::TemporaryRedirect:
    return "307 Temporary Redirect";

  case Status::PermanentRedirect:
    return "308 Permanent Redirect";

  case Status::BadRequest:
    return "400 Bad Request";

  case Status::Unauthorized:
    return "401 Unauthorized";

  case Status::PaymentRequired:
    return "402 Payment Required";

  case Status::Forbidden:
    return "403 Forbidden";

  case Status::NotFound:
    return "404 Not Found";

  case Status::MethodNotAllowed:
    return "405 Method Not Allowed";

  case Status::NotAcceptable:
    return "406 Not Acceptable";

  case Status::ProxyAuthenticationRequired:
    return "407 Proxy Authentication Required";

  case Status::RequestTimeout:
    return "408 Request Timeout";

  case Status::Conflict:
    return "409 Conflict";

  case Status::Gone:
    return "410 Gone";

  case Status::LengthRequired:
    return "411 Length Required";

  case Status::PreconditionFailed:
    return "412 Precondition Failed";

  case Status::PayloadTooLarge:
    return "413 Payload Too Large";

  case Status::UriTooLong:
    return "414 URI Too Long";

  case Status::UnsupportedMediaType:
    return "415 Unsupported Media Type";

  case Status::RangeNotSatisfiable:
    return "416 Range Not Satisfiable";

  case Status::ExpectationFailed:
    return "417 Expectation Failed";

  case Status::ImATeapot:
    return "418 I'm a teapot";

  case Status::MisdirectedRequest:
    return "421 Misdirected Request";

  case Status::UnprocessableContent:
    return "422 Unprocessable Content";

  case Status::Locked:
    return "423 Locked";

  case Status::FailedDependency:
    return "424 Failed Dependency";

  case Status::TooEarly:
    return "425 Too Early";

  case Status::UpgradeRequired:
    return "426 Upgrade Required";

  case Status::PreconditionRequired:
    return "428 Precondition Required";

  case Status::TooManyRequests:
    return "429 Too Many Requests";

  case Status::RequestHeaderFieldsTooLarge:
    return "431 Request Header Fields Too Large";

  case Status::UnavailableForLegalReasons:
    return "451 Unavailable For Legal Reasons";

  case Status::InternalServerError:
    return "500 Internal Server Error";

  case Status::NotImplemented:
    return "501 Not Implemented";

  case Status::BadGateway:
    return "502 Bad Gateway";

  case Status::ServiceUnavailable:
    return "503 Service Unavailable";

  case Status::GatewayTimeout:
    return "504 Gateway Timeout";

  case Status::HttpVersionNotSupported:
    return "505 HTTP Version Not Supported";

  case Status::VariantAlsoNegotiates:
    return "506 Variant Also Negotiates";

  case Status::InsufficientStorage:
    return "507 Insufficient Storage";

  case Status::LoopDetected:
    return "508 Loop Detected";

  case Status::NotExtended:
    return "510 Not Extended";

  case Status::NetworkAuthenticationRequired:
    return "511 Network Authentication Required";
  }
}
