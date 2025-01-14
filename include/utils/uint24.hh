#ifndef NETCPP_UTILS_UINT24_HH
#define NETCPP_UTILS_UINT24_HH

struct uint24 {
  unsigned int data : 24;

  uint24(unsigned int data) : data{data} {}
} __attribute__((packed));

#endif // NETCPP_UTILS_UINT24_HH
