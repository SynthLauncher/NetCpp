#ifndef NETCPP_HTTP2_TABLES_HH
#define NETCPP_HTTP2_TABLES_HH

#include <array>
#include <string>

struct Table {
  unsigned int index;
  std::string headerName;
  std::string headerValue;

  Table() {}

  Table(unsigned int index, std::string headerName)
      : index{index}, headerName{headerName} {}

  Table(unsigned int index, std::string headerName, std::string headerValue)
      : index{index}, headerName{headerName}, headerValue{headerValue} {}
};

const std::array<Table, 61> staticTable = {
    Table(1, ":authority"),
    Table(2, ":method", "GET"),
    Table(3, ":method", "POST"),
    Table(4, ":path", "/"),
    Table(5, ":path", "/index.html"),
    Table(6, ":scheme", "http"),
    Table(7, ":scheme", "https"),
    Table(8, ":status", "200"),
    Table(9, ":status", "204"),
    Table(10, ":status", "206"),
    Table(11, ":status", "304"),
    Table(12, ":status", "400"),
    Table(13, ":status", "404"),
    Table(14, ":status", "500"),
    Table(15, "accept-charset"),
    Table(16, "accept-encoding", "gzip, deflate"),
    Table(17, "accept-language"),
    Table(18, "accept-ranges"),
    Table(19, "accept"),
    Table(20, "access-control-allow-origin"),
    Table(21, "age"),
    Table(22, "allow"),
    Table(23, "authorization"),
    Table(24, "cache-control"),
    Table(25, "content-disposition"),
    Table(26, "content-encoding"),
    Table(27, "content-language"),
    Table(28, "content-length"),
    Table(29, "content-location"),
    Table(30, "content-range"),
    Table(31, "content-type"),
    Table(32, "cookie"),
    Table(33, "date"),
    Table(34, "etag"),
    Table(35, "expect"),
    Table(36, "expires"),
    Table(37, "from"),
    Table(38, "host"),
    Table(39, "if-match"),
    Table(40, "if-modified-since"),
    Table(41, "if-none-match"),
    Table(42, "if-range"),
    Table(43, "if-unmodified-since"),
    Table(44, "last-modified"),
    Table(45, "link"),
    Table(46, "location"),
    Table(47, "max-forwards"),
    Table(48, "proxy-authenticate"),
    Table(49, "proxy-authorization"),
    Table(50, "range"),
    Table(51, "referer"),
    Table(52, "refresh"),
    Table(53, "retry-after"),
    Table(54, "server"),
    Table(55, "set-cookie"),
    Table(56, "strict-transport-security"),
    Table(57, "transfer-encoding"),
    Table(58, "user-agent"),
    Table(59, "vary"),
    Table(60, "via"),
    Table(61, "www-authenticate"),
};

#endif // NETCPP_HTTP2_TABLES_HH
