#include "include/http2/frames.hh"
#include <cmath>
#include <cstdint>

template <typename T>
T calcSize(const std::vector<bit> &bits) {
  T res = 0;

  for (size_t i = 0; i < bits.size(); ++i) {
    if (bits[i] == 1) {
      res += std::pow(2, 23 - i);
    }
  }

  return res;
}

DataFrame::DataFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  paddedFlag = bits[36];
  endStreamFlag = bits[39];

  streamIdentifier = calcSize<uint32_t>(
      std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  if (paddedFlag) {
    padLength = calcSize<uint8_t>(
        std::vector<bit>(bits.begin() + 73, bits.begin() + 81));
    data = std::vector<bit>(bits.begin() + 82, bits.end() - padLength);
    padding = std::vector<uint8_t>(bits.end() - padLength, bits.end());
  } else {
    data = std::vector<bit>(bits.begin() + 73, bits.end());
  }
}

HeaderFrame::HeaderFrame(const std::vector<bit> &bits) {
  length = calcSize<uint24>(std::vector<bit>(bits.begin(), bits.begin() + 24));

  priorityFlag = bits[35];
  paddedFlag = bits[37];
  endHeaderFlag = bits[38];
  endStreamFlag = bits[40];

  streamIdentifier = calcSize<uint32_t>(
      std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  size_t index = 73;

  if (paddedFlag) {
    padLength = calcSize<uint8_t>(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 8));
    padding = std::vector<uint8_t>(bits.end() - padLength, bits.end());
    index += 9;
  }

  if (priorityFlag) {
    exclusive = bits[index++];
    streamDependency = calcSize<uint32_t>(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 31));
    index += 33;
    weight = calcSize<uint8_t>(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 8));
    index += 9;
  }

  if (paddedFlag) {
    fieldBlockFragment =
        std::vector<bit>(bits.begin() + index, bits.end() - padLength);
  } else {
    fieldBlockFragment = std::vector<bit>(bits.begin() + index, bits.end());
  }
}

PriorityFrame::PriorityFrame(const std::vector<bit> &bits) {
  streamIdentifier = calcSize<uint32_t>(
      std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  exclusive = bits[73];
  streamDependency = calcSize<uint32_t>(
      std::vector<bit>(bits.begin() + 74, bits.begin() + 105));

  weight = calcSize<uint8_t>(std::vector<bit>(bits.begin() + 106, bits.end()));
}

RstStreamFrame::RstStreamFrame(const std::vector<bit> &bits) {
  streamIdentifier = calcSize<uint32_t>(
      std::vector<bit>(bits.begin() + 41, bits.begin() + 72));

  errorCode = calcSize<uint32_t>(std::vector<bit>(bits.end() - 32, bits.end()));
}
