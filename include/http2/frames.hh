#ifndef NETCPP_HTTP2_FRAMES_HH
#define NETCPP_HTTP2_FRAMES_HH

#include "utils/uint24.hh"
#include <array>
#include <cstdint>
#include <vector>

#define HTTP2_CONNECTION_PREFACE                                               \
  0x505249202a20485454502f322e300d0a0d0a534d0d0a0d0a

using bit = bool;

inline int calcPadding(size_t payloadLength) {
  return (8 - (payloadLength % 8)) % 8;
}

struct Frame {
  uint24 length;

  const bit reserved = 0;
  uint32_t streamIdentifier;
};

struct DataFrame : public Frame {
  const uint8_t type = 0x00;
  const std::array<bit, 4> unusedFlag = {0, 0, 0, 0};
  bit paddedFlag;
  const std::array<bit, 2> unusedFlag2 = {0, 0};
  bit endStreamFlag;

  std::array<bit, 8> padLength;
  std::vector<bit> data;
  std::vector<bit> padding;

  DataFrame(std::vector<bit> bits);
};

struct HeaderFrame : public Frame {
  const uint8_t type = 0x01;
  const std::array<bit, 2> unusedFlag = {0, 0};
  bit priorityFlag;
  const bit unusedFlag2 = 0;
  bit paddedFlag;
  bit endHeaderFlag;
  const bit unusedFlag3 = 0;
  bit endStreamFlag;

  std::array<bit, 8> padLength;
  bit exclusive;
  std::array<bit, 31> streamDependency;
  std::array<bit, 8> weight;
  std::vector<bit> fieldBlockFragment;
  std::vector<bit> padding;

  HeaderFrame(std::vector<bit> bits);
};

struct PriorityFrame : public Frame {
  uint24 length = 0x05;
  const uint8_t type = 0x02;
  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  bit exclusive;
  std::array<bit, 31> streamDependency;
  std::array<bit, 8> weight;

  PriorityFrame(std::vector<bit> bits);
};

struct RstStreamFrame : public Frame {
  uint24 length = 0x04;
  const uint8_t type = 0x03;
  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};
  std::array<bit, 32> errorCode;

  RstStreamFrame(std::vector<bit> bits);
};

struct Setting {
  std::array<bit, 16> identifier;
  std::array<bit, 32> value;
};

struct SettingFrame : public Frame {
  const uint8_t type = 0x04;
  const std::array<bit, 7> unusedFlag = {0, 0, 0, 0, 0, 0, 0};
  bit ackFlag;

  uint32_t streamIdentifier = 0;

  std::vector<Setting> settings;

  SettingFrame(std::vector<bit> bits);
};

struct PushPromiseFrame : public Frame {
  const uint8_t type = 0x05;
  const std::array<bit, 4> unusedFlag = {0, 0, 0, 0};
  bit paddedFlag;
  bit endHeaderFlag;
  const std::array<bit, 2> unusedFlag2 = {0, 0};

  std::array<bit, 8> padLength;
  const bit reserved2 = 0;
  std::array<bit, 31> promiseStreamId;
  std::vector<bit> fieldBlockFragment;
  std::vector<bit> padding;

  PushPromiseFrame(std::vector<bit> bits);
};

struct PingFrame : public Frame {
  uint24 length = 0x08;
  const uint8_t type = 0x06;
  const std::array<bit, 7> unusedFlag = {0, 0, 0, 0, 0, 0, 0};
  bit ackFlag;

  uint32_t streamIdentifier = 0;

  std::array<bit, 64> opaqueData;

  PingFrame(std::vector<bit> bits);
};

struct GoawayFrame : public Frame {
  const uint8_t type = 0x07;
  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  const bit reserved2 = 0;
  std::array<bit, 31> lastStreamId;
  std::array<bit, 32> errorCode;
  std::vector<bit> additionalDebugData;

  uint32_t streamIdentifier = 0;

  GoawayFrame(std::vector<bit> bits);
};

struct WindowUpdateFrame : public Frame {
  uint24 length = 0x04;
  const uint8_t type = 0x08;
  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  const bit reserved2 = 0;
  std::array<bit, 31> windowSizeIncrement;

  WindowUpdateFrame(std::vector<bit> bits);
};

struct ContinuationFrame : public Frame {
  const uint8_t type = 0x09;
  const std::array<bit, 5> unusedFlag = {0, 0, 0, 0, 0};

  std::vector<bit> fieldBlockFragment;
};

#endif // NETCPP_HTTP2_FRAMES_HH
