/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_CONGESTION_CONTROLLER_NETWORK_CONTROL_UNITS_DATA_SIZE_H_
#define MODULES_CONGESTION_CONTROLLER_NETWORK_CONTROL_UNITS_DATA_SIZE_H_

#include <stdint.h>
#include <cmath>
#include <limits>
#include <string>

#include "rtc_base/checks.h"

namespace webrtc {
namespace data_size_impl {
constexpr int64_t kPlusInfinityVal = std::numeric_limits<int64_t>::max();
constexpr int64_t kNotInitializedVal = -1;
}  // namespace data_size_impl

// DataSize is a class represeting a count of bytes. Note that while it can be
// initialized by a number of bits, it does not guarantee that the resolution is
// kept and the internal storage is in bytes. The number of bits will be
// truncated to fit.
class DataSize {
 public:
  DataSize() : DataSize(data_size_impl::kNotInitializedVal) {}
  static DataSize Zero() { return DataSize(0); }
  static DataSize Infinity() {
    return DataSize(data_size_impl::kPlusInfinityVal);
  }
  static DataSize bytes(int64_t bytes) {
    RTC_DCHECK_GE(bytes, 0);
    return DataSize(bytes);
  }
  static DataSize bits(int64_t bits) {
    RTC_DCHECK_GE(bits, 0);
    return DataSize(bits / 8);
  }
  int64_t bytes() const {
    RTC_DCHECK(IsFinite());
    return bytes_;
  }
  int64_t kilobytes() const { return (bytes() + 500) / 1000; }
  int64_t bits() const { return bytes() * 8; }
  int64_t kilobits() const { return (bits() + 500) / 1000; }
  bool IsZero() const { return bytes_ == 0; }
  bool IsInfinite() const { return bytes_ == data_size_impl::kPlusInfinityVal; }
  bool IsInitialized() const {
    return bytes_ != data_size_impl::kNotInitializedVal;
  }
  bool IsFinite() const { return IsInitialized() && !IsInfinite(); }
  DataSize operator-(const DataSize& other) const {
    return DataSize::bytes(bytes() - other.bytes());
  }
  DataSize operator+(const DataSize& other) const {
    return DataSize::bytes(bytes() + other.bytes());
  }
  DataSize operator*(double scalar) const {
    return DataSize::bytes(std::round(bytes() * scalar));
  }
  DataSize operator*(int64_t scalar) const {
    return DataSize::bytes(bytes() * scalar);
  }
  DataSize operator*(int32_t scalar) const {
    return DataSize::bytes(bytes() * scalar);
  }
  DataSize operator/(int64_t scalar) const {
    return DataSize::bytes(bytes() / scalar);
  }
  DataSize& operator-=(const DataSize& other) {
    bytes_ -= other.bytes();
    return *this;
  }
  DataSize& operator+=(const DataSize& other) {
    bytes_ += other.bytes();
    return *this;
  }
  bool operator==(const DataSize& other) const {
    return bytes_ == other.bytes_;
  }
  bool operator!=(const DataSize& other) const {
    return bytes_ != other.bytes_;
  }
  bool operator<=(const DataSize& other) const {
    return bytes_ <= other.bytes_;
  }
  bool operator>=(const DataSize& other) const {
    return bytes_ >= other.bytes_;
  }
  bool operator>(const DataSize& other) const { return bytes_ > other.bytes_; }
  bool operator<(const DataSize& other) const { return bytes_ < other.bytes_; }

 private:
  explicit DataSize(int64_t bytes) : bytes_(bytes) {}
  int64_t bytes_;
};
inline DataSize operator*(const double& scalar, const DataSize& size) {
  return size * scalar;
}
inline DataSize operator*(const int64_t& scalar, const DataSize& size) {
  return size * scalar;
}
inline DataSize operator*(const int32_t& scalar, const DataSize& size) {
  return size * scalar;
}

std::string ToString(const DataSize& value);

}  // namespace webrtc

#endif  // MODULES_CONGESTION_CONTROLLER_NETWORK_CONTROL_UNITS_DATA_SIZE_H_
