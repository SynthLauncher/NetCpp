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

template <typename T>
T calcSize(const std::vector<bit> &bits);

struct DataFrame {
  uint24 length;
  const uint8_t type = 0x00;

  const std::array<bit, 4> unusedFlag = {0, 0, 0, 0};
  bit paddedFlag;
  const std::array<bit, 2> unusedFlag2 = {0, 0};
  bit endStreamFlag;

  const bit reserved = 0;
  uint32_t streamIdentifier;

  uint8_t padLength;
  std::vector<bit> data;
  std::vector<uint8_t> padding;

  DataFrame(const std::vector<bit> &bits);
};

struct HeaderFrame {
  uint24 length;
  const uint8_t type = 0x01;

  const std::array<bit, 2> unusedFlag = {0, 0};
  bit priorityFlag;
  const bit unusedFlag2 = 0;
  bit paddedFlag;
  bit endHeaderFlag;
  const bit unusedFlag3 = 0;
  bit endStreamFlag;

  const bit reserved = 0;
  uint32_t streamIdentifier;

  uint8_t padLength;
  bit exclusive;
  uint32_t streamDependency;
  uint8_t weight;
  std::vector<bit> fieldBlockFragment;
  std::vector<uint8_t> padding;

  HeaderFrame(const std::vector<bit> &bits);
};

struct PriorityFrame {
  uint24 length;
  const uint8_t type = 0x02;

  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  const bit reserved = 0;
  uint32_t streamIdentifier;

  bit exclusive;
  uint32_t streamDependency;
  uint8_t weight;

  PriorityFrame(const std::vector<bit> &bits);
};

struct RstStreamFrame {
  const uint24 length = 0x04;
  const uint8_t type = 0x03;

  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  const bit reserved = 0;
  uint32_t streamIdentifier;

  uint32_t errorCode;

  RstStreamFrame(const std::vector<bit> &bits);
};

struct Setting {
  uint16_t identifier;
  uint32_t value;

  Setting(uint16_t identifier, uint32_t value)
      : identifier{identifier}, value{value} {}
};

struct SettingFrame {
  uint24 length;
  const uint8_t type = 0x04;

  const std::array<bit, 7> unusedFlag = {0, 0, 0, 0, 0, 0, 0};
  bit ackFlag;

  const bit reserved = 0;
  const uint32_t streamIdentifier = 0;

  std::vector<Setting> settings;

  SettingFrame(const std::vector<bit> &bits);
};

struct PushPromiseFrame {
  uint24 length;
  const uint8_t type = 0x05;

  const std::array<bit, 4> unusedFlag = {0, 0, 0, 0};
  bit paddedFlag;
  bit endHeaderFlag;
  const std::array<bit, 2> unusedFlag2 = {0, 0};

  const bit reserved = 0;
  uint32_t streamIdentifier;

  uint8_t padLength;
  const bit reserved2 = 0;
  uint32_t promiseStreamId;
  std::vector<bit> fieldBlockFragment;
  std::vector<uint8_t> padding;

  PushPromiseFrame(const std::vector<bit> &bits);
};

struct PingFrame {
  uint24 length = 0x08;

  const uint8_t type = 0x06;
  const std::array<bit, 7> unusedFlag = {0, 0, 0, 0, 0, 0, 0};
  bit ackFlag;

  const bit reserved = 0;
  const uint32_t streamIdentifier = 0;

  uint64_t opaqueData;

  PingFrame(const std::vector<bit> &bits);
};

struct GoawayFrame {
  uint24 length;
  const uint8_t type = 0x07;

  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  const bit reserved = 0;
  const uint32_t streamIdentifier = 0;

  const bit reserved2 = 0;
  uint32_t lastStreamId;
  uint32_t errorCode;
  std::vector<bit> additionalDebugData;

  GoawayFrame(const std::vector<bit> &bits);
};

struct WindowUpdateFrame {
  const uint24 length = 0x04;
  const uint8_t type = 0x08;

  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  const bit reserved = 0;
  uint32_t streamIdentifier;

  const bit reserved2 = 0;
  std::array<bit, 31> windowSizeIncrement;

  WindowUpdateFrame(const std::vector<bit> &bits);
};

struct ContinuationFrame {
  uint24 length;
  const uint8_t type = 0x09;

  const bit reserved = 0;
  uint32_t streamIdentifier;

  const std::array<bit, 5> unusedFlag = {0, 0, 0, 0, 0};

  std::vector<bit> fieldBlockFragment;
};

#endif // NETCPP_HTTP2_FRAMES_HH
