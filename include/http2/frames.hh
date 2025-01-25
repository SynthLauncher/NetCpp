#ifndef NETCPP_HTTP2_FRAMES_HH
#define NETCPP_HTTP2_FRAMES_HH

#include "include/utils/uint24.hh"
#include "include/utils/uint31.hh"
#include <climits>
#include <cmath>
#include <cstdint>
#include <type_traits>
#include <vector>

#define HTTP2_CONNECTION_PREFACE                                               \
  0x505249202a20485454502f322e300d0a0d0a534d0d0a0d0a

#define BASIC_MAX_FRAME_SIZE 16384

using bit = bool;

template <typename T>
inline void fillBinary(T input, std::vector<bit> &bits) {
  size_t size = std::is_same<T, uint31>::value ? 31 : sizeof(T) * CHAR_BIT;
  for (size_t i = size - 1; i != SIZE_MAX; --i) {
    bits.push_back(input & (1 << i) ? 1 : 0);
  }
}

inline uint8_t calcPadding(const size_t payloadLength) {
  return (8 - (payloadLength % 8)) % 8;
}

template <typename T>
T calcSize(const std::vector<bit> &bits);

struct DataFrame {
  uint24 length;

  bit paddedFlag;
  bit endStreamFlag;

  uint31 streamIdentifier;

  uint8_t padLength = 0;
  std::vector<bit> data;

  DataFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct HeaderFrame {
  uint24 length;

  bit priorityFlag;
  bit paddedFlag;
  bit endHeaderFlag;
  bit endStreamFlag;

  uint31 streamIdentifier;

  uint8_t padLength = 0;
  bit exclusive;
  uint31 streamDependency;
  uint8_t weight;
  std::vector<bit> fieldBlockFragment;

  HeaderFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct PriorityFrame {
  const uint24 length = 5;

  uint31 streamIdentifier;

  bit exclusive;
  uint31 streamDependency;
  uint8_t weight;

  PriorityFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct RstStreamFrame {
  const uint24 length = 0x04;

  uint31 streamIdentifier;

  uint32_t errorCode;

  RstStreamFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct Setting {
  uint16_t identifier;
  uint32_t value;

  Setting(uint16_t identifier, uint32_t value)
      : identifier{identifier}, value{value} {}
  std::vector<bit> toBits();
};

struct SettingFrame {
  uint24 length;

  bit ackFlag;

  const uint31 streamIdentifier = 0;

  std::vector<Setting> settings;

  SettingFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct PushPromiseFrame {
  uint24 length;

  bit paddedFlag;
  bit endHeaderFlag;

  uint31 streamIdentifier;

  uint8_t padLength = 0;
  uint31 promiseStreamId;
  std::vector<bit> fieldBlockFragment;

  PushPromiseFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct PingFrame {
  uint24 length = 0x08;

  bit ackFlag;

  const uint31 streamIdentifier = 0;

  uint64_t opaqueData;

  PingFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct GoawayFrame {
  uint24 length;

  const uint31 streamIdentifier = 0;

  uint31 lastStreamId;
  uint32_t errorCode;
  std::vector<bit> additionalDebugData;

  GoawayFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct WindowUpdateFrame {
  const uint24 length = 0x04;

  uint31 streamIdentifier;

  uint32_t windowSizeIncrement;

  WindowUpdateFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

struct ContinuationFrame {
  uint24 length;

  bit endHeaderFlag;

  uint31 streamIdentifier;

  std::vector<bit> fieldBlockFragment;

  ContinuationFrame(const std::vector<bit> &bits);
  std::vector<bit> toBits();
};

#endif // NETCPP_HTTP2_FRAMES_HH
