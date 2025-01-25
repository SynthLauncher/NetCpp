#include "include/http2/frames.hh"
#include <climits>

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
  } else {
    data = std::vector<bit>(bits.begin() + 72, bits.end());
  }
}

std::vector<bit> DataFrame::toBits() {
  std::vector<bit> bits;

  length = data.size() / 8;

  if (paddedFlag) {
    padLength = calcPadding(data.size());
    length += (8 + padLength * CHAR_BIT) / 8;
  }

  fillBinary<uint24>(length, bits);
  bits.insert(bits.end(), 8, 0);

  bits.insert(bits.end(), 4, 0);
  bits.push_back(paddedFlag);
  bits.insert(bits.end(), 2, 0);
  bits.push_back(endStreamFlag);

  bits.push_back(0);
  fillBinary<uint31>(streamIdentifier, bits);

  if (paddedFlag) {
    fillBinary<uint8_t>(padLength, bits);
  }
  bits.insert(bits.end(), data.begin(), data.end());
  bits.insert(bits.end(), padLength * CHAR_BIT, 0);

  return bits;
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

std::vector<bit> HeaderFrame::toBits() {
  std::vector<bit> bits;

  length = fieldBlockFragment.size() / 8;

  if (paddedFlag) {
    padLength = calcPadding(fieldBlockFragment.size());
    length += (8 + padLength * CHAR_BIT) / 8;
  }

  if (priorityFlag) {
    length += 6;
  }

  fillBinary<uint24>(length, bits);
  fillBinary<uint8_t>(1, bits);

  bits.insert(bits.end(), 2, 0);
  bits.push_back(priorityFlag);
  bits.push_back(0);
  bits.push_back(paddedFlag);
  bits.push_back(endHeaderFlag);
  bits.push_back(0);
  bits.push_back(endStreamFlag);

  bits.push_back(0);
  fillBinary<uint31>(streamIdentifier, bits);

  if (paddedFlag) {
    fillBinary<uint8_t>(padLength, bits);
  }

  if (priorityFlag) {
    bits.push_back(exclusive);
    fillBinary<uint31>(streamDependency, bits);
    fillBinary<uint8_t>(weight, bits);
  }

  bits.insert(bits.end(), fieldBlockFragment.begin(), fieldBlockFragment.end());
  bits.insert(bits.end(), padLength * CHAR_BIT, 0);

  return bits;
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
      calcSize<uint31>(std::vector<bit>(bits.begin() + 73, bits.begin() + 104));
  errorCode = calcSize<uint32_t>(
      std::vector<bit>(bits.begin() + 104, bits.begin() + 136));
  additionalDebugData = std::vector<bit>(bits.begin() + 136, bits.end());
}

WindowUpdateFrame::WindowUpdateFrame(const std::vector<bit> &bits) {
  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  windowSizeIncrement =
      calcSize<uint32_t>(std::vector<bit>(bits.end() - 32, bits.end()));
}

ContinuationFrame::ContinuationFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  endHeaderFlag = bits[37];

  streamIdentifier =
      calcSize<uint31>(std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  fieldBlockFragment = std::vector<bit>(bits.begin() + 72, bits.end());
}
