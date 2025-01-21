#include "include/http2/frames.hh"
#include "include/utils/uint31.hh"
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>

TEST(Http2, FillBinaryUint8) {
  std::vector<bit> bits;
  std::vector<bit> expectResult = {0, 0, 0, 0, 0, 1, 0, 1};
  uint8_t number = 5;

  fillBinary(number, bits);

  ASSERT_EQ(bits, expectResult);
}

TEST(Http2, DataFrameParseWithoutPadding) {
  std::vector<bit> bits;
  std::vector<bit> expectData{0, 1, 1, 0, 0, 0, 1, 1};

  fillBinary<uint24>(1, bits);
  bits.insert(bits.end(), 8, 0);
  bits.insert(bits.end(), 4, 0);
  bits.push_back(0);
  bits.insert(bits.end(), 2, 0);
  bits.push_back(1);
  bits.push_back(0);
  fillBinary<uint31>(3, bits);
  bits.erase(bits.begin() + 41);
  fillBinary<unsigned char>('c', bits);

  DataFrame frame{bits};

  EXPECT_EQ(frame.length, 1);
  EXPECT_EQ(frame.type, 0);
  EXPECT_EQ(frame.paddedFlag, 0);
  EXPECT_EQ(frame.endStreamFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 3);
  EXPECT_EQ(frame.data, expectData);
}
