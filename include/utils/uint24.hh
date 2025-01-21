#ifndef NETCPP_UTILS_UINT24_HH
#define NETCPP_UTILS_UINT24_HH

struct uint24 {
  unsigned int data : 24;

  uint24() {}
  uint24(unsigned int data) : data{data} {}

  inline uint24 &operator+=(const int sn) {
    data += sn;
    return *this;
  }

  inline friend bool operator==(const uint24 &lhs, const uint24 &rhs) {
    return lhs.data == rhs.data;
  }

  template <typename T>
  inline bool operator&(const T &sn) {
    return data & sn;
  }
} __attribute__((packed));

#endif // NETCPP_UTILS_UINT24_HH
