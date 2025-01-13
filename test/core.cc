#include "include/request.hh"
#include "include/response.hh"
#include "include/utils/http_version.hh"
#include "include/utils/method.hh"
#include <gtest/gtest.h>
#include <string>

TEST(CoreTest, RequestTest) {
  std::string request =
      "POST /submit-form HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: "
      "application/x-www-form-urlencoded\r\nContent-Length: "
      "27\r\n\r\nname=JohnDoe&age=30&city=Paris";

  HttpRequest req(request);

  ASSERT_EQ(req.method, Method::POST);
  ASSERT_EQ(req.uri, "/submit-form");
  ASSERT_EQ(req.version, HttpVersion::HTTP1);
  ASSERT_EQ(req.body, "name=JohnDoe&age=30&city=Paris");

  ASSERT_EQ(req.headers["Host"], "www.example.com");
  ASSERT_EQ(req.headers["Content-Type"], "application/x-www-form-urlencoded");
  ASSERT_EQ(req.headers["Content-Length"], "27");
}

TEST(CoreTest, ResponseTest) {
  std::string response =
      "HTTP/1.1 200 OK\r\nDate: Sat, 05 Jan 2025 12:00:00 GMT\r\nServer: "
      "Apache/2.4.41 (Ubuntu)\r\nContent-Type: text/html; "
      "charset=UTF-8\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, "
      "world!";

  HttpResponse res(response);

  ASSERT_EQ(res.version, HttpVersion::HTTP1);
  ASSERT_EQ(res.status, Status::Ok);
  ASSERT_EQ(res.body, "Hello, world!");

  ASSERT_EQ(res.headers["Date"], "Sat, 05 Jan 2025 12:00:00 GMT");
  ASSERT_EQ(res.headers["Server"], "Apache/2.4.41 (Ubuntu)");
  ASSERT_EQ(res.headers["Content-Type"], "text/html; charset=UTF-8");
  ASSERT_EQ(res.headers["Content-Length"], "13");
  ASSERT_EQ(res.headers["Connection"], "close");
}
