#include "include/http2/hpack.hh"

int calcBits(const std::vector<bit> &bits) {
  int res = 0;

  for (size_t i = 0; i < bits.size(); ++i) {
    if (bits[i] == 1) {
      res += std::pow(2, bits.size() - i - 1);
    }
  }

  return res;
}

std::string bitsToString(const std::vector<bit> &bits) {
  std::string result;

  for (size_t i = 0; i < bits.size() / CHAR_BIT; ++i) {
    int begin = i * CHAR_BIT;
    int byte = calcBits(
        std::vector<bit>(bits.begin() + begin, bits.begin() + begin + 8));

    result.push_back(static_cast<char>(byte));
  }

  return result;
}

void parseHeader(const std::vector<bit> &bits, ClientHeader &header) {
  size_t index = 0;

  while (index < bits.size()) {
    if (bits[index] == 1) {
      ++index;
      int staticIndex =
          calcBits(std::vector(bits.begin() + index, bits.begin() + index + 7));
      index += 7;

      if (staticIndex == 0 || staticIndex > 61) {
        // TODO: HANDLE ERROR
      }

      header.headerList.push_back(staticTable[staticIndex - 1]);
    } else if (std::vector<bit>(bits.begin() + index,
                                bits.begin() + index + 2) ==
               LITERAL_INCREMENTAL_INDEXING_PREFIX) {
      index += 2;

      Table table =
          parseHeaderIncrementalIndexing(index, bits, ++header.lastIndex);

      header.dynamicTable.push_back(table);

    } else if (std::vector<bit>(bits.begin() + index,
                                bits.begin() + index + 4) ==
               LITERAL_WITHOUT_INDEXING_PREFIX) {

      index += 4;
      Table table = parseHeaderWithoutIndex(index, bits);

      header.withoutIndexed.push_back(table);

    } else if (std::vector<bit>(bits.begin() + index,
                                bits.begin() + index + 4) ==
               LITERAL_NEVER_INDEXING_PREFIX) {

      index += 4;
      Table table = parseHeaderWithoutIndex(index, bits);

      header.neverIndexed.push_back(table);
    } else {
      // TODO: HANDLE ERROR
    }
  }
}

Table parseHeaderIncrementalIndexing(size_t &index,
                                     const std::vector<bit> &bits,
                                     unsigned int incHeader) {
  int headerIndex = calcBits(
      std::vector<bit>(bits.begin() + index, bits.begin() + index + 6));
  index += 6;

  Table table;
  table.index = incHeader;

  if (headerIndex == 0) {
    if (bits[index++] == 1) {
      // TODO: HANDLE WITH HUFFMAN CODE
    } else {
      size_t nameLength = calcBits(
          std::vector<bit>(bits.begin() + index, bits.begin() + index + 7));
      index += 7;

      table.headerName = bitsToString(std::vector<bit>(
          bits.begin() + index, bits.begin() + index + nameLength * CHAR_BIT));

      index += nameLength * CHAR_BIT;
    }
  } else if (headerIndex < 61) {
    table.headerName = staticTable[headerIndex - 1].headerName;
  } else {
    // TODO: HANDLE ERROR
  }

  if (bits[index++] == 1) {
    // TODO: HANDLE WITH HUFFMAN CODE
  } else {
    size_t nameLength = calcBits(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 7));
    index += 7;

    table.headerValue = bitsToString(std::vector<bit>(
        bits.begin() + index, bits.begin() + index + nameLength * CHAR_BIT));

    index += nameLength * CHAR_BIT;
  }

  return table;
}

Table parseHeaderWithoutIndex(size_t &index, const std::vector<bit> &bits) {
  int headerIndex = calcBits(
      std::vector<bit>(bits.begin() + index, bits.begin() + index + 4));
  index += 4;

  Table table;

  if (headerIndex == 0) {
    if (bits[index++] == 1) {
      // TODO: HANDLE WITH HUFFMAN CODE
    } else {
      size_t nameLength = calcBits(
          std::vector<bit>(bits.begin() + index, bits.begin() + index + 7));
      index += 7;

      table.headerName = bitsToString(std::vector<bit>(
          bits.begin() + index, bits.begin() + index + nameLength * CHAR_BIT));

      index += nameLength * CHAR_BIT;
    }
  } else if (headerIndex < 61) {
    table.headerName = staticTable[headerIndex - 1].headerName;
  } else {
    // TODO: HANDLE ERROR
  }

  if (bits[index++] == 1) {
    // TODO: HANDLE WITH HUFFMAN CODE
  } else {
    size_t nameLength = calcBits(
        std::vector<bit>(bits.begin() + index, bits.begin() + index + 7));
    index += 7;

    table.headerValue = bitsToString(std::vector<bit>(
        bits.begin() + index, bits.begin() + index + nameLength * CHAR_BIT));

    index += nameLength * CHAR_BIT;
  }

  return table;
}
