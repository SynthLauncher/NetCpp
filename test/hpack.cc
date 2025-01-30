#include "include/http2/hpack.hh"
#include "include/http2/frames.hh"
#include "include/http2/tables.hh"
#include <gtest/gtest.h>

TEST(Hpack, IncrementalIndexingWithIndexWithoutHuffman) {
  std::vector<bit> bits = {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                           1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1,
                           0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0};

  size_t index = 2;

  Table table = parseHeaderIncrementalIndexing(index, bits, 62);

  ASSERT_EQ(table.index, 62);
  ASSERT_EQ(table.headerName, ":method");
  ASSERT_EQ(table.headerValue, "PUT");
}

TEST(Hpack, IncrementalIndexingWithoutIndexWithoutHuffman) {
  std::vector<bit> bits = {
      0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
      0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0};

  size_t index = 2;

  Table table = parseHeaderIncrementalIndexing(index, bits, 62);

  ASSERT_EQ(table.index, 62);
  ASSERT_EQ(table.headerName, "foo");
  ASSERT_EQ(table.headerValue, "bar");
}

TEST(Hpack, WithoutIndexingWithIndexWithoutHuffman) {
  std::vector<bit> bits = {
      0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1,
      0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0,
      0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0};

  size_t index = 4;

  Table table = parseHeaderWithoutIndex(index, bits);

  ASSERT_EQ(table.headerName, ":path");
  ASSERT_EQ(table.headerValue, "/ok.txt");
}

TEST(Hpack, WithoutIndexingWithoutIndexWithoutHuffman) {
  std::vector<bit> bits = {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
      0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0};

  size_t index = 4;

  Table table = parseHeaderWithoutIndex(index, bits);

  ASSERT_EQ(table.headerName, "foo");
  ASSERT_EQ(table.headerValue, "bar");
}

TEST(Hpack, NeverIndexingWithIndexWithoutHuffman) {
  std::vector<bit> bits = {
      0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1,
      0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0,
      0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0};

  size_t index = 4;

  Table table = parseHeaderWithoutIndex(index, bits);

  ASSERT_EQ(table.headerName, ":path");
  ASSERT_EQ(table.headerValue, "/ok.txt");
}

TEST(Hpack, NeverIndexingWithoutIndexWithoutHuffman) {
  std::vector<bit> bits = {
      0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
      0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0};

  size_t index = 4;

  Table table = parseHeaderWithoutIndex(index, bits);

  ASSERT_EQ(table.headerName, "foo");
  ASSERT_EQ(table.headerValue, "bar");
}
