#ifndef NETCPP_HTTP2_HPACK_HH
#define NETCPP_HTTP2_HPACK_HH

#include "include/http2/frames.hh"
#include "include/http2/tables.hh"
#include <climits>
#include <string>
#include <vector>

#define TABLE_MAX_SIZE 4096
#define LITERAL_INCREMENTAL_INDEXING_PREFIX std::vector<bit>{0, 1}
#define LITERAL_WITHOUT_INDEXING_PREFIX std::vector<bit>{0, 0, 0, 0}
#define LITERAL_NEVER_INDEXING_PREFIX std::vector<bit>{0, 0, 0, 1}
#define DYNAMIC_TABLE_SIZE_UPDATE std::vector<bit>{0, 0, 1}

struct ClientHeader {
  int lastIndex = 61;
  std::vector<Table> headerList;
  std::vector<Table> dynamicTable;
  std::vector<Table> withoutIndexed;
  std::vector<Table> neverIndexed;
};

int calcBits(const std::vector<bit> &bits);

std::string bitsToString(const std::vector<bit> &bits);

void parseHeader(const std::vector<bit> &bits, ClientHeader &header);

Table parseHeaderIncrementalIndexing(size_t &index,
                                     const std::vector<bit> &bits,
                                     unsigned int incHeader);

Table parseHeaderWithoutIndex(size_t &index, const std::vector<bit> &bits);

#endif // NETCPP_HTTP2_HPACK_HH
