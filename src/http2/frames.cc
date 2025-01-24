#include "include/http2/frames.hh"

template <typename T>
T calcSize(const std::vector<bit> &bits) {
  T res = 0;
  size_t size;

  if (std::is_same<T, uint31>::value) {
    size = 31;
  } else {
    size = sizeof(T) * CHAR_BIT;
  }

  for (size_t i = 0; i < size; ++i) {
    if (bits[i] == 1) {
      res += std::pow(2, size - i - 1);
    }
  }

  return res;
}

DataFrame::DataFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  paddedFlag = bits[36];
  endStreamFlag = bits[39];

  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  if (paddedFlag) {
    padLength = calcSize<uint8_t>(
        std::vector<bit>(bits.begin() + 72, bits.begin() + 80));

    size_t padLengthInBit = padLength * CHAR_BIT;
    data = std::vector<bit>(bits.begin() + 80, bits.end() - padLengthInBit);
    padding = std::vector<uint8_t>(padLengthInBit, 0);
  } else {
    data = std::vector<bit>(bits.begin() + 72, bits.end());
  }
}

HeaderFrame::HeaderFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  priorityFlag = bits[34];
  paddedFlag = bits[36];
  endHeaderFlag = bits[37];
  endStreamFlag = bits[39];

  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  size_t index = 72;

  if (paddedFlag) {
    padLength = calcSize<uint8_t>(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 8));
    padding = std::vector<uint8_t>(padLength * CHAR_BIT, 0);
    index += 8;
  }

  if (priorityFlag) {
    exclusive = bits[index++];
    streamDependency = calcSize<uint31>(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 31));
    index += 31;
    weight = calcSize<uint8_t>(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 8));
    index += 8;
  }

  if (paddedFlag) {
    fieldBlockFragment = std::vector<bit>(bits.begin() + index,
                                          bits.end() - padLength * CHAR_BIT);
  } else {
    fieldBlockFragment = std::vector<bit>(bits.begin() + index, bits.end());
  }
}

PriorityFrame::PriorityFrame(const std::vector<bit> &bits) {
  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  exclusive = bits[72];
  streamDependency =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 73, bits.end() - 8));

  weight = calcSize<uint8_t>(std::vector<bit>(bits.end() - 8, bits.end()));
}

RstStreamFrame::RstStreamFrame(const std::vector<bit> &bits) {
  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  errorCode = calcSize<uint32_t>(std::vector<bit>(bits.end() - 32, bits.end()));
}

SettingFrame::SettingFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  ackFlag = bits[39];

  size_t boucleTour = (bits.end() - (bits.begin() + 72)) / 48;
  size_t begin = 72;

  for (size_t i = 0; i < boucleTour; ++i) {
    begin += i * 48;
    uint16_t identifier = calcSize<uint16_t>(
        std::vector<bit>(bits.begin() + begin, bits.begin() + begin + 16));
    uint32_t value = calcSize<uint32_t>(
        std::vector<bit>(bits.begin() + begin + 16, bits.begin() + begin + 48));

    settings.push_back(Setting(identifier, value));
  }
}

PushPromiseFrame::PushPromiseFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  paddedFlag = bits[36];
  endHeaderFlag = bits[37];

  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  size_t index = 72;

  if (paddedFlag) {
    padLength = calcSize<uint8_t>(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 8));
    padding = std::vector<uint8_t>(padLength * CHAR_BIT, 0);
    index += 8;
  }

  ++index;

  promiseStreamId = calcSize<uint31>(
      std::vector<bit>(bits.begin() + index, bits.begin() + index + 31));
  index += 31;

  if (paddedFlag) {
    fieldBlockFragment = std::vector<bit>(bits.begin() + index,
                                          bits.end() - padLength * CHAR_BIT);
  } else {
    fieldBlockFragment = std::vector<bit>(bits.begin() + index, bits.end());
  }
}

PingFrame::PingFrame(const std::vector<bit> &bits) {
  ackFlag = bits[39];

  opaqueData =
      calcSize<uint64_t>(std::vector<bit>(bits.end() - 64, bits.end()));
}

GoawayFrame::GoawayFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  lastStreamId =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 74, bits.begin() + 105));
  errorCode = calcSize<uint32_t>(
      std::vector<bit>(bits.begin() + 105, bits.begin() + 137));
  additionalDebugData = std::vector<bit>(bits.begin() + 137, bits.end());
}

WindowUpdateFrame::WindowUpdateFrame(const std::vector<bit> &bits) {
  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  windowSizeIncrement =
      calcSize<uint32_t>(std::vector<bit>(bits.end() - 32, bits.end()));
}

ContinuationFrame::ContinuationFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  endHeaderFlag = bits[38];

  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  fieldBlockFragment = std::vector<bit>(bits.begin() + 72, bits.end());
}
