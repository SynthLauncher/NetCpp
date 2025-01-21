#ifndef NETCPP_UTILS_UINT31_HH
#define NETCPP_UTILS_UINT31_HH

struct uint31 {
  unsigned int data : 31;

  uint31() {}
  uint31(unsigned int data) : data{data} {}

  inline uint31 &operator+=(const int sn) {
    data += sn;
    return *this;
  }

  inline friend bool operator==(const uint31 &lhs, const uint31 &rhs) {
    return lhs.data == rhs.data;
  }

  template <typename T>
  inline bool operator&(const T &sn) {
    return data & sn;
  }
} __attribute__((packed));

#endif // NETCPP_UTILS_UINT31_HH
