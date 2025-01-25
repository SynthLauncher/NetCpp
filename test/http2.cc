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

TEST(Http2, FillBinaryUint16) {
  std::vector<bit> bits;
  std::vector<bit> expectResult = {0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 0, 0, 0, 1, 0, 1};
  uint16_t number = 5;

  fillBinary(number, bits);

  ASSERT_EQ(bits.size(), 16);
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

TEST(Http2, CalcSizeUint16) {
  uint16_t shouldBeResult = 432;
  std::vector<bit> dataTest;
  fillBinary(shouldBeResult, dataTest);

  uint16_t result = calcSize<uint16_t>(dataTest);

  ASSERT_EQ(dataTest.size(), 16);
  ASSERT_EQ(result, shouldBeResult);
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
}

TEST(Http2, HeaderFrameWithoutPaddingWithoutPriority) {
  std::vector<bit> bits;
  std::vector<bit> expectFieldFrame = {0, 0, 1, 0, 0, 1, 1, 0};

  // Length, Type
  fillBinary<uint24>(8, bits);
  fillBinary<uint8_t>(1, bits);

  // Flag
  bits.insert(bits.end(), 2, 0);
  bits.push_back(0);
  bits.push_back(0);
  bits.push_back(0);
  bits.push_back(1);
  bits.push_back(0);
  bits.push_back(1);

  // Stream Depedency
  bits.push_back(0);
  fillBinary<uint31>(3, bits);

  // Field Block Fragment
  bits.insert(bits.end(), expectFieldFrame.begin(), expectFieldFrame.end());

  HeaderFrame frame{bits};

  EXPECT_EQ(frame.length, 8);
  EXPECT_EQ(frame.type, 1);
  EXPECT_EQ(frame.priorityFlag, 0);
  EXPECT_EQ(frame.paddedFlag, 0);
  EXPECT_EQ(frame.endHeaderFlag, 1);
  EXPECT_EQ(frame.endStreamFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 3);
  EXPECT_EQ(frame.fieldBlockFragment, expectFieldFrame);
}

TEST(Http2, HeaderFrameWithPaddingWithoutPriority) {
  std::vector<bit> bits;
  std::vector<bit> expectFieldFrame = {0, 0, 1, 0, 0, 1, 1, 0};

  // Length, Type
  fillBinary<uint24>(8, bits);
  fillBinary<uint8_t>(1, bits);

  // Flag
  bits.insert(bits.end(), 2, 0);
  bits.push_back(0);
  bits.push_back(0);
  bits.push_back(1);
  bits.push_back(1);
  bits.push_back(0);
  bits.push_back(1);

  // Stream Depedency
  bits.push_back(0);
  fillBinary<uint31>(3, bits);

  // Field Block Fragment, Padding
  fillBinary<uint8_t>(calcPadding(1), bits);
  bits.insert(bits.end(), expectFieldFrame.begin(), expectFieldFrame.end());
  bits.insert(bits.end(), calcPadding(1) * CHAR_BIT, 0);

  HeaderFrame frame{bits};

  EXPECT_EQ(frame.length, 8);
  EXPECT_EQ(frame.type, 1);
  EXPECT_EQ(frame.priorityFlag, 0);
  EXPECT_EQ(frame.paddedFlag, 1);
  EXPECT_EQ(frame.endHeaderFlag, 1);
  EXPECT_EQ(frame.endStreamFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 3);
  EXPECT_EQ(frame.padLength, 7);
  EXPECT_EQ(frame.fieldBlockFragment, expectFieldFrame);
}

TEST(Http2, HeaderFrameWithoutPaddingWithPriority) {
  std::vector<bit> bits;
  std::vector<bit> expectFieldFrame = {0, 0, 1, 0, 0, 1, 1, 0};

  // Length, Type
  fillBinary<uint24>(8, bits);
  fillBinary<uint8_t>(1, bits);

  // Flag
  bits.insert(bits.end(), 2, 0);
  bits.push_back(1);
  bits.push_back(0);
  bits.push_back(0);
  bits.push_back(1);
  bits.push_back(0);
  bits.push_back(1);

  // Stream Depedency
  bits.push_back(0);
  fillBinary<uint31>(3, bits);

  // Field Block Fragment, Priority fields
  bits.push_back(0);
  fillBinary<uint31>(43, bits);
  fillBinary<uint8_t>(1, bits);
  bits.insert(bits.end(), expectFieldFrame.begin(), expectFieldFrame.end());

  HeaderFrame frame{bits};

  EXPECT_EQ(frame.length, 8);
  EXPECT_EQ(frame.type, 1);
  EXPECT_EQ(frame.priorityFlag, 1);
  EXPECT_EQ(frame.paddedFlag, 0);
  EXPECT_EQ(frame.endHeaderFlag, 1);
  EXPECT_EQ(frame.endStreamFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 3);
  EXPECT_EQ(frame.exclusive, 0);
  EXPECT_EQ(frame.streamDependency, 43);
  EXPECT_EQ(frame.weight, 1);
  EXPECT_EQ(frame.fieldBlockFragment, expectFieldFrame);
}

TEST(Http2, HeaderFrameWithPaddingWithPriority) {
  std::vector<bit> bits;
  std::vector<bit> expectFieldFrame = {0, 0, 1, 0, 0, 1, 1, 0};

  // Length, Type
  fillBinary<uint24>(8, bits);
  fillBinary<uint8_t>(1, bits);

  // Flag
  bits.insert(bits.end(), 2, 0);
  bits.push_back(1);
  bits.push_back(0);
  bits.push_back(1);
  bits.push_back(1);
  bits.push_back(0);
  bits.push_back(1);

  // Stream Depedency
  bits.push_back(0);
  fillBinary<uint31>(3, bits);

  // Field Block Fragment, Padding, Priority fields
  fillBinary<uint8_t>(calcPadding(1), bits);
  bits.push_back(0);
  fillBinary<uint31>(43, bits);
  fillBinary<uint8_t>(1, bits);
  bits.insert(bits.end(), expectFieldFrame.begin(), expectFieldFrame.end());
  bits.insert(bits.end(), calcPadding(1) * CHAR_BIT, 0);

  HeaderFrame frame{bits};

  EXPECT_EQ(frame.length, 8);
  EXPECT_EQ(frame.type, 1);
  EXPECT_EQ(frame.priorityFlag, 1);
  EXPECT_EQ(frame.paddedFlag, 1);
  EXPECT_EQ(frame.endHeaderFlag, 1);
  EXPECT_EQ(frame.endStreamFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 3);
  EXPECT_EQ(frame.padLength, 7);
  EXPECT_EQ(frame.exclusive, 0);
  EXPECT_EQ(frame.streamDependency, 43);
  EXPECT_EQ(frame.weight, 1);
  EXPECT_EQ(frame.fieldBlockFragment, expectFieldFrame);
}

TEST(Http2, PriorityFrame) {
  std::vector<bit> bits;

  // Length, Type
  fillBinary<uint24>(5, bits);
  fillBinary<uint8_t>(2, bits);

  // Flag
  bits.insert(bits.end(), 8, 0);

  // Stream Identifier
  bits.push_back(0);
  fillBinary<uint31>(4, bits);

  // Exclusive, Stream Depedency, Weight
  bits.push_back(0);
  fillBinary<uint31>(43, bits);
  fillBinary<uint8_t>(2, bits);

  PriorityFrame frame{bits};

  EXPECT_EQ(frame.length, 5);
  EXPECT_EQ(frame.type, 2);
  EXPECT_EQ(frame.streamIdentifier, 4);
  EXPECT_EQ(frame.exclusive, 0);
  EXPECT_EQ(frame.streamDependency, 43);
  EXPECT_EQ(frame.weight, 2);
}

TEST(Http2, RstStreamFrame) {
  std::vector<bit> bits;

  // Length, Type
  fillBinary<uint24>(4, bits);
  fillBinary<uint8_t>(3, bits);

  // Flag
  bits.insert(bits.end(), 8, 0);

  // Steam Depedency
  bits.push_back(0);
  fillBinary<uint31>(5, bits);

  // Error Code
  bits.insert(bits.end(), 32, 0);

  RstStreamFrame frame{bits};

  EXPECT_EQ(frame.length, 4);
  EXPECT_EQ(frame.type, 3);
  EXPECT_EQ(frame.streamIdentifier, 5);
  EXPECT_EQ(frame.errorCode, 0);
}

TEST(Http2, SettingsFrameWithSingleSetting) {
  std::vector<bit> bits;

  // Length, Type
  fillBinary<uint24>(6, bits);
  fillBinary<uint8_t>(4, bits);

  // Flag
  bits.insert(bits.end(), 7, 0);
  bits.push_back(1);

  // Stream Identifier
  bits.insert(bits.end(), 32, 0);

  // Setting
  Setting setting{2, 0};
  fillBinary<uint16_t>(setting.identifier, bits);
  fillBinary<uint32_t>(setting.value, bits);

  SettingFrame frame{bits};

  EXPECT_EQ(frame.length, 6);
  EXPECT_EQ(frame.type, 4);
  EXPECT_EQ(frame.ackFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 0);
  EXPECT_EQ(frame.settings[0].identifier, 2);
  EXPECT_EQ(frame.settings[0].value, 0);
}

TEST(Http2, SettingsFrameWithMultipleSetting) {
  std::vector<bit> bits;

  // Length, Type
  fillBinary<uint24>(6, bits);
  fillBinary<uint8_t>(4, bits);

  // Flag
  bits.insert(bits.end(), 7, 0);
  bits.push_back(1);

  // Stream Identifier
  bits.insert(bits.end(), 32, 0);

  // Setting
  Setting setting{2, 0};
  Setting setting2{3, 1};
  fillBinary(setting.identifier, bits);
  fillBinary(setting.value, bits);
  fillBinary(setting2.identifier, bits);
  fillBinary(setting2.value, bits);

  SettingFrame frame{bits};

  EXPECT_EQ(frame.length, 6);
  EXPECT_EQ(frame.type, 4);
  EXPECT_EQ(frame.ackFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 0);
  ASSERT_EQ(frame.settings.size(), 2);
  EXPECT_EQ(frame.settings[0].identifier, 2);
  EXPECT_EQ(frame.settings[0].value, 0);
  EXPECT_EQ(frame.settings[1].identifier, 3);
  EXPECT_EQ(frame.settings[1].value, 1);
}

TEST(Http2, PushPromiseWithoutPadding) {
  std::vector<bit> bits;
  std::vector<bit> expectFieldFrame = {0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1};

  // Length, Type
  fillBinary<uint24>(5, bits);
  fillBinary<uint8_t>(5, bits);

  // Flag
  bits.insert(bits.end(), 4, 0);
  bits.push_back(0);
  bits.push_back(1);
  bits.insert(bits.end(), 2, 0);

  // Stream Depedency
  bits.push_back(0);
  fillBinary<uint31>(43, bits);

  // Promise stream, field block fragment
  bits.push_back(0);
  fillBinary<uint31>(55, bits);
  bits.insert(bits.end(), expectFieldFrame.begin(), expectFieldFrame.end());

  PushPromiseFrame frame{bits};

  EXPECT_EQ(frame.length, 5);
  EXPECT_EQ(frame.type, 5);
  EXPECT_EQ(frame.paddedFlag, 0);
  EXPECT_EQ(frame.endHeaderFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 43);
  EXPECT_EQ(frame.promiseStreamId, 55);
  EXPECT_EQ(frame.fieldBlockFragment, expectFieldFrame);
}

TEST(Http2, PushPromiseWithPadding) {
  std::vector<bit> bits;
  std::vector<bit> expectFieldFrame = {0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1};

  // Length, Type
  fillBinary<uint24>(5, bits);
  fillBinary<uint8_t>(5, bits);

  // Flag
  bits.insert(bits.end(), 4, 0);
  bits.push_back(1);
  bits.push_back(1);
  bits.insert(bits.end(), 2, 0);

  // Stream Depedency
  bits.push_back(0);
  fillBinary<uint31>(43, bits);

  // Promise stream, field block fragment
  fillBinary<uint8_t>(calcPadding(5), bits);
  bits.push_back(0);
  fillBinary<uint31>(55, bits);
  bits.insert(bits.end(), expectFieldFrame.begin(), expectFieldFrame.end());
  bits.insert(bits.end(), calcPadding(5) * CHAR_BIT, 0);

  PushPromiseFrame frame{bits};

  EXPECT_EQ(frame.length, 5);
  EXPECT_EQ(frame.type, 5);
  EXPECT_EQ(frame.paddedFlag, 1);
  EXPECT_EQ(frame.endHeaderFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 43);
  EXPECT_EQ(frame.promiseStreamId, 55);
  EXPECT_EQ(frame.fieldBlockFragment, expectFieldFrame);
}

TEST(Http2, PingFrame) {
  std::vector<bit> bits;

  // Length, Type
  fillBinary<uint24>(8, bits);
  fillBinary<uint8_t>(6, bits);

  // Flag
  bits.insert(bits.end(), 7, 0);
  bits.push_back(1);

  // Stream Depedency
  bits.push_back(0);
  bits.insert(bits.end(), 31, 0);

  // Opaque Data
  bits.insert(bits.end(), 64, 0);

  PingFrame frame{bits};

  EXPECT_EQ(frame.length, 8);
  EXPECT_EQ(frame.type, 6);
  EXPECT_EQ(frame.ackFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 0);
  EXPECT_EQ(frame.opaqueData, 0);
}

TEST(Http2, GoawayFrameWithoutData) {
  std::vector<bit> bits;

  // Length, Type
  fillBinary<uint24>(3, bits);
  fillBinary<uint8_t>(7, bits);

  // Flag
  bits.insert(bits.end(), 8, 0);

  // Stream Depedency
  bits.push_back(0);
  bits.insert(bits.end(), 31, 0);

  // Last Stream Id, ErrorCode, Debug Data
  bits.push_back(0);
  fillBinary<uint31>(4, bits);
  bits.insert(bits.end(), 32, 0);

  GoawayFrame frame{bits};

  EXPECT_EQ(frame.length, 3);
  EXPECT_EQ(frame.type, 7);
  EXPECT_EQ(frame.streamIdentifier, 0);
  EXPECT_EQ(frame.lastStreamId, 4);
  EXPECT_EQ(frame.errorCode, 0);
  ASSERT_EQ(frame.additionalDebugData.size(), 0);
}

TEST(Http2, GoawayFrameWithData) {
  std::vector<bit> bits;
  std::vector<bit> expectDebugData = {0, 1, 0, 1, 1, 0, 1, 0, 0};

  // Length, Type
  fillBinary<uint24>(3, bits);
  fillBinary<uint8_t>(7, bits);

  // Flag
  bits.insert(bits.end(), 8, 0);

  // Stream Depedency
  bits.push_back(0);
  bits.insert(bits.end(), 31, 0);

  // Last Stream Id, ErrorCode, Debug Data
  bits.push_back(0);
  fillBinary<uint31>(4, bits);
  bits.insert(bits.end(), 32, 0);
  bits.insert(bits.end(), expectDebugData.begin(), expectDebugData.end());

  GoawayFrame frame{bits};

  EXPECT_EQ(frame.length, 3);
  EXPECT_EQ(frame.type, 7);
  EXPECT_EQ(frame.streamIdentifier, 0);
  EXPECT_EQ(frame.lastStreamId, 4);
  EXPECT_EQ(frame.errorCode, 0);
  ASSERT_EQ(frame.additionalDebugData.size(), 9);
  EXPECT_EQ(frame.additionalDebugData, expectDebugData);
}

TEST(Http2, WindowUpdateFrame) {
  std::vector<bit> bits;

  // Length, Type
  fillBinary<uint24>(4, bits);
  fillBinary<uint8_t>(8, bits);

  // Flag
  bits.insert(bits.end(), 8, 0);

  // Stream Identifier
  bits.push_back(0);
  fillBinary<uint31>(3, bits);

  // Window Size Increment
  bits.push_back(0);
  fillBinary<uint31>(5235, bits);

  WindowUpdateFrame frame{bits};

  EXPECT_EQ(frame.length, 4);
  EXPECT_EQ(frame.type, 8);
  EXPECT_EQ(frame.streamIdentifier, 3);
  EXPECT_EQ(frame.windowSizeIncrement, 5235);
}

TEST(Http2, ContinuationFrame) {
  std::vector<bit> bits;
  std::vector<bit> expectFieldFrame = {0, 1, 1, 0, 0, 1, 0, 1, 0};

  // Length, Type
  fillBinary<uint24>(4, bits);
  fillBinary<uint8_t>(9, bits);

  // Flag
  bits.insert(bits.end(), 5, 0);
  bits.push_back(1);
  bits.insert(bits.end(), 2, 0);

  // Stream Identifier
  bits.push_back(0);
  fillBinary<uint31>(62, bits);

  // Field Block Fragment
  bits.insert(bits.end(), expectFieldFrame.begin(), expectFieldFrame.end());

  ContinuationFrame frame{bits};

  EXPECT_EQ(frame.length, 4);
  EXPECT_EQ(frame.type, 9);
  EXPECT_EQ(frame.endHeaderFlag, 1);
  EXPECT_EQ(frame.streamIdentifier, 62);
  EXPECT_EQ(frame.fieldBlockFragment, expectFieldFrame);
}
