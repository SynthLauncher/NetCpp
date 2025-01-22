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

  ASSERT_EQ(bits.size(), 8);
  ASSERT_EQ(bits, expectResult);
}

TEST(Http2, FillBinaryUint24) {
  std::vector<bit> bits;
  std::vector<bit> expectResult = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1};
  uint24 number = 5;

  fillBinary(number, bits);

  ASSERT_EQ(bits.size(), 24);
  ASSERT_EQ(bits, expectResult);
}

TEST(Http2, FillBinaryUint31) {
  std::vector<bit> bits;
  std::vector<bit> expectResult = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 0, 1, 0, 1};
  uint31 number = 5;

  fillBinary(number, bits);

  ASSERT_EQ(bits.size(), 31);
  ASSERT_EQ(bits, expectResult);
}

TEST(Http2, FillBinaryUint32) {
  std::vector<bit> bits;
  std::vector<bit> expectResult = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 0, 0, 1, 0, 1};
  uint32_t number = 5;

  fillBinary(number, bits);

  ASSERT_EQ(bits.size(), 32);
  ASSERT_EQ(bits, expectResult);
}

TEST(Http2, CalcSizeUint8) {
  const std::vector<bit> dataTest = {0, 0, 0, 0, 0, 1, 0, 0};

  uint8_t result = calcSize<uint8_t>(dataTest);

  ASSERT_EQ(dataTest.size(), 8);
  ASSERT_EQ(result, 4);
}

TEST(Http2, CalcSizeUint24) {
  uint24 shouldBeResult = 15;
  std::vector<bit> dataTest;
  fillBinary(shouldBeResult, dataTest);

  uint24 result = calcSize<uint24>(dataTest);

  ASSERT_EQ(dataTest.size(), 24);
  ASSERT_EQ(result, shouldBeResult);
}

TEST(Http2, CalcSizeUint31) {
  uint31 shouldBeResult = 142;
  std::vector<bit> dataTest;
  fillBinary(shouldBeResult, dataTest);

  uint31 result = calcSize<uint31>(dataTest);

  ASSERT_EQ(dataTest.size(), 31);
  ASSERT_EQ(result, shouldBeResult);
}

TEST(Http2, CalcSizeUint32) {
  uint32_t shouldBeResult = 312;
  std::vector<bit> dataTest;
  fillBinary(shouldBeResult, dataTest);

  uint32_t result = calcSize<uint32_t>(dataTest);

  ASSERT_EQ(dataTest.size(), 32);
  ASSERT_EQ(result, shouldBeResult);
}

TEST(Http2, DataFrameParseWithoutPadding) {
  std::vector<bit> bits;
  std::vector<bit> expectData{0, 1, 1, 0, 0, 0, 1, 1};

  // Length, Type
  fillBinary<uint24>(1, bits);
  bits.insert(bits.end(), 8, 0);

  // Flag
  bits.insert(bits.end(), 4, 0);
  bits.push_back(0);
  bits.insert(bits.end(), 2, 0);
  bits.push_back(1);

  // Stream Depedency
  bits.push_back(0);
  fillBinary<uint31>(3, bits);

  // Data
  fillBinary<unsigned char>('c', bits);

  DataFrame frame{bits};

  EXPECT_EQ(frame.length, 1);
  EXPECT_EQ(frame.type, 0);
  EXPECT_EQ(frame.paddedFlag, 0);
  EXPECT_EQ(frame.endStreamFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 3);
  EXPECT_EQ(frame.data, expectData);
}

TEST(Http2, DataFrameParseWithPadding) {
  std::vector<bit> bits;
  std::vector<bit> expectData{0, 1, 1, 0, 0, 0, 1, 1};

  // Length, Type
  fillBinary<uint24>(56, bits);
  bits.insert(bits.end(), 8, 0);

  // Flag
  bits.insert(bits.end(), 4, 0);
  bits.push_back(1);
  bits.insert(bits.end(), 2, 0);
  bits.push_back(1);

  // Stream Depedency
  bits.push_back(0);
  fillBinary<uint31>(3, bits);

  // Padding, Data
  fillBinary<uint8_t>(calcPadding(1), bits);
  fillBinary<unsigned char>('c', bits);
  bits.insert(bits.end(), calcPadding(1) * CHAR_BIT, 0);

  DataFrame frame{bits};

  EXPECT_EQ(frame.length, 56);
  EXPECT_EQ(frame.type, 0);
  EXPECT_EQ(frame.paddedFlag, 1);
  EXPECT_EQ(frame.endStreamFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 3);
  EXPECT_EQ(frame.padLength, 7);
  EXPECT_EQ(frame.data, expectData);
  EXPECT_EQ(frame.padding.size(), 7 * CHAR_BIT);
}
