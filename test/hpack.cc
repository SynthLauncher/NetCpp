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

TEST(Hpack, MultipleHeaderType) {
  std::vector<bit> bits = {
      1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1,
      0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
      0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1,
      0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1,
      0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0,
      0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
      0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1,
      0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0};

  ClientHeader clientHeader;
  decodeHeader(bits, clientHeader);

  ASSERT_EQ(clientHeader.lastIndex, 62);
  ASSERT_EQ(clientHeader.headerList.size(), 2);
  ASSERT_EQ(clientHeader.dynamicTable.size(), 1);
  ASSERT_EQ(clientHeader.withoutIndexed.size(), 0);
  ASSERT_EQ(clientHeader.neverIndexed.size(), 1);

  EXPECT_EQ(clientHeader.headerList[0].index, 2);
  EXPECT_EQ(clientHeader.headerList[0].headerName, ":method");
  EXPECT_EQ(clientHeader.headerList[0].headerValue, "GET");

  EXPECT_EQ(clientHeader.headerList[1].index, 5);
  EXPECT_EQ(clientHeader.headerList[1].headerName, ":path");
  EXPECT_EQ(clientHeader.headerList[1].headerValue, "/index.html");

  EXPECT_EQ(clientHeader.dynamicTable[0].index, 62);
  EXPECT_EQ(clientHeader.dynamicTable[0].headerName, "foo");
  EXPECT_EQ(clientHeader.dynamicTable[0].headerValue, "bar");

  EXPECT_EQ(clientHeader.neverIndexed[0].headerName, "authorization");
  EXPECT_EQ(clientHeader.neverIndexed[0].headerValue, "secret");
}
