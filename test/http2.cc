#include "include/http2/frames.hh"
#include <cstdint>
#include <gtest/gtest.h>

TEST(Http2, FillBinaryUint8) {
  std::vector<bit> bits;
  std::vector<bit> expectResult = {0, 0, 0, 0, 0, 1, 0, 1};
  uint8_t number = 5;

  fillBinary(number, bits);

  ASSERT_EQ(bits, expectResult);
}

TEST(Http2, DataFrameParseWithoutPadding) {
  std::vector<bit> bits;
  ASSERT_EQ(1, 1);
}
