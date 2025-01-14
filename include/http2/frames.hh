#ifndef NETCPP_HTTP2_FRAMES_HH
#define NETCPP_HTTP2_FRAMES_HH

#include "utils/uint24.hh"
#include <array>
#include <vector>

#define HTTP2_CONNECTION_PREFACE                                               \
  0x505249202a20485454502f322e300d0a0d0a534d0d0a0d0a

using bit = bool;

enum class Type {
  DATA,
  HEADERS,
  PRIORITY,
  RST_STREAM,
  SETTINGS,
  PUSH_PROMISE,
  PING,
  GOAWAY,
  WINDOWS_UPDATE,
  CONTINUATION
};

struct Frame {
  uint24 length;
  Type type;

  const bit reserved = 0;
  std::array<bit, 31> streamIdentifier;
};

struct DataFrame : public Frame {
  const std::array<bit, 4> unusedFlag = {0, 0, 0, 0};
  bit paddedFlag;
  const std::array<bit, 2> unusedFlag2 = {0, 0};
  bit endStreamFlag;

  std::array<bit, 8> padLength;
  std::vector<bit> data;
  std::vector<bit> padding;
};

struct HeaderFrame : public Frame {
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
};

struct PriorityFrame : public Frame {
  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  bit exclusive;
  std::array<bit, 31> streamDependency;
  std::array<bit, 8> weight;
};

struct rstStreamFrame : public Frame {
  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};
  std::array<bit, 32> errorCode;
};

struct Setting {
  std::array<bit, 16> identifier;
  std::array<bit, 32> value;
};

struct SettingFrame : public Frame {
  const std::array<bit, 7> unusedFlag = {0, 0, 0, 0, 0, 0, 0};
  bit ackFlag;

  std::vector<Setting> settings;
};

struct PushPromiseFrame : public Frame {
  const std::array<bit, 4> unusedFlag = {0, 0, 0, 0};
  bit paddedFlag;
  bit endHeaderFlag;
  const std::array<bit, 2> unusedFlag2 = {0, 0};

  std::array<bit, 8> padLength;
  const bit reserved2 = 0;
  std::array<bit, 31> promiseStreamId;
  std::vector<bit> fieldBlockFragment;
  std::vector<bit> padding;
};

struct PingFrame : public Frame {
  const std::array<bit, 7> unusedFlag = {0, 0, 0, 0, 0, 0, 0};
  bit ackFlag;

  std::array<bit, 64> opaqueData;
};

struct GoawayFrame : public Frame {
  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  const bit reserved2 = 0;
  std::array<bit, 31> lastStreamId;
  std::array<bit, 32> errorCode;
  std::vector<bit> additionalDebugData;
};

struct WindowUpdateFrame : public Frame {
  const std::array<bit, 8> unusedFlag = {0, 0, 0, 0, 0, 0, 0, 0};

  const bit reserved2 = 0;
  std::array<bit, 31> windowSizeIncrement;
};

#endif // NETCPP_HTTP2_FRAMES_HH
