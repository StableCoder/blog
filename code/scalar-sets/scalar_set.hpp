/*
    Copyright (C) 2018 George Cave - gcave@stablecoder.ca

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef STEC_ENUMERATED_SCALAR_SET_HPP
#define STEC_ENUMERATED_SCALAR_SET_HPP

#include <algorithm>
#include <array>
#include <cstdint>

namespace stec {

/// \brief A template for use for tying together a bunch of scalar variables,
/// performing access with an enum class. \tparam T The underlying type of the
/// template (ex int, float, etc.) \tparam EnumClass The enum type to use, must
/// be zero-based and be in a solid incremental block. \tparam NumValues The
/// number of values held in the template
///
/// The EnumeratedScalarSet is to make stat storage easier, where similar-type
/// stored scalar values can be stored and accessed either individually or the
/// total set of stats can be interacted on as a whole. Support also exists for
/// exchanging the stored values with StatTemplate types that have a different
/// scalar types, but must have the same EnumClass and NumValues.
///
/// This template also takes a parameter for an Enum class, allowing more
/// restricted/codified access to the elements.
template <typename T, class EnumClass, int NumValues>
class EnumeratedScalarSet {
  static_assert(
      std::is_scalar<T>::value,
      "EnumeratedScalarSet - Template parameter T must be of scalar type.");

public:
  /// \brief Initial value constructor
  /// \param initial This is the value the template vars are set to.
  EnumeratedScalarSet(T initial = 0) noexcept;

  /// \brief Destructor
  ~EnumeratedScalarSet() noexcept = default;

  /// \brief Copy constructor
  EnumeratedScalarSet(const EnumeratedScalarSet &) noexcept = default;

  /// \brief Copy operator
  EnumeratedScalarSet &
  operator=(const EnumeratedScalarSet &) noexcept = default;

  /// \brief Move constructor
  EnumeratedScalarSet(EnumeratedScalarSet &&) noexcept = default;

  /// \brief Move operator
  EnumeratedScalarSet &operator=(EnumeratedScalarSet &&) noexcept = default;

  /// \brief Other-typed template copy-constructor.
  /// \param initial The other type to copy-construct over.
  template <typename Y>
  EnumeratedScalarSet(
      EnumeratedScalarSet<Y, EnumClass, NumValues> initial) noexcept;

  /// \brief Returns a reference to the underlying value, denoted by the index.
  /// \return A reference to the value.
  T &operator[](const EnumClass) noexcept;

  /// \brief Returns a const reference to the underlying value, denoted by the
  /// index. \return A const reference to the value.
  T operator[](const EnumClass) const noexcept;

  template <typename Y>
  bool operator==(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) const
      noexcept;

  template <typename Y>
  bool operator!=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) const
      noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> &
  operator+=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> &
  operator-=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> &
  operator*=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> &
  operator/=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues>
  operator+(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues>
  operator-(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues>
  operator*(const EnumeratedScalarSet<Y, EnumClass, NumValues> &)noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues>
  operator/(const EnumeratedScalarSet<Y, EnumClass, NumValues> &) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> &operator+=(const Y) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> &operator-=(const Y) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> &operator*=(const Y) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> &operator/=(const Y) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> operator+(const Y) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> operator-(const Y) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> operator*(const Y) noexcept;

  template <typename Y>
  EnumeratedScalarSet<T, EnumClass, NumValues> operator/(const Y) noexcept;

  /// \brief Clamps the minimum value of the internals to the given parameter.
  /// \param min The value that all values will be clamped to a minimum of.
  void clampMin(T min) noexcept;

  /// \brief Clamps the maximum value of internals to the given parameter.
  /// \param max The value that all values will be clamped to a maximum of.
  void clampMax(T max) noexcept;

private:
  /// The actual array of stored stat values.
  std::array<T, NumValues> stats;
};

template <typename T, class EnumClass, int NumValues>
EnumeratedScalarSet<T, EnumClass, NumValues>::EnumeratedScalarSet(
    T initial) noexcept
    : stats{} {
  std::fill_n(stats.data(), NumValues, initial);
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>::EnumeratedScalarSet(
    EnumeratedScalarSet<Y, EnumClass, NumValues> initial) noexcept
    : stats{} {
  for (int i = 0; i < NumValues; i++) {
    stats[i] = initial[static_cast<EnumClass>(i)];
  }
}

template <typename T, class EnumClass, int NumValues>
T &EnumeratedScalarSet<T, EnumClass, NumValues>::
operator[](const EnumClass rhs) noexcept {
  return stats[static_cast<typename std::underlying_type<EnumClass>::type>(
      rhs)];
}

template <typename T, class EnumClass, int NumValues>
T EnumeratedScalarSet<T, EnumClass, NumValues>::
operator[](const EnumClass rhs) const noexcept {
  return stats[static_cast<typename std::underlying_type<EnumClass>::type>(
      rhs)];
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
bool EnumeratedScalarSet<T, EnumClass, NumValues>::
operator==(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) const
    noexcept {
  for (int i = 0; i < NumValues; i++) {
    if (stats[i] != rhs[static_cast<EnumClass>(i)]) {
      return false;
    }
  }

  return true;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
bool EnumeratedScalarSet<T, EnumClass, NumValues>::
operator!=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) const
    noexcept {
  return !(*this == rhs);
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues> &
EnumeratedScalarSet<T, EnumClass, NumValues>::
operator+=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] += rhs[static_cast<EnumClass>(i)];
  }

  return *this;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues> &
EnumeratedScalarSet<T, EnumClass, NumValues>::
operator-=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] -= rhs[static_cast<EnumClass>(i)];
  }

  return *this;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues> &
EnumeratedScalarSet<T, EnumClass, NumValues>::
operator*=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] *= rhs[static_cast<EnumClass>(i)];
  }

  return *this;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues> &
EnumeratedScalarSet<T, EnumClass, NumValues>::
operator/=(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] /= rhs[static_cast<EnumClass>(i)];
  }

  return *this;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>
EnumeratedScalarSet<T, EnumClass, NumValues>::
operator+(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) noexcept {
  return EnumeratedScalarSet<T, EnumClass, NumValues>(*this) += rhs;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>
EnumeratedScalarSet<T, EnumClass, NumValues>::
operator-(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) noexcept {
  return EnumeratedScalarSet<T, EnumClass, NumValues>(*this) -= rhs;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>
    EnumeratedScalarSet<T, EnumClass, NumValues>::operator*(
        const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) noexcept {
  return EnumeratedScalarSet<T, EnumClass, NumValues>(*this) *= rhs;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>
EnumeratedScalarSet<T, EnumClass, NumValues>::
operator/(const EnumeratedScalarSet<Y, EnumClass, NumValues> &rhs) noexcept {
  return EnumeratedScalarSet<T, EnumClass, NumValues>(*this) /= rhs;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues> &
EnumeratedScalarSet<T, EnumClass, NumValues>::operator+=(const Y rhs) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] += rhs;
  }

  return *this;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues> &
EnumeratedScalarSet<T, EnumClass, NumValues>::operator-=(const Y rhs) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] -= rhs;
  }

  return *this;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues> &
EnumeratedScalarSet<T, EnumClass, NumValues>::operator*=(const Y rhs) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] *= rhs;
  }

  return *this;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues> &
EnumeratedScalarSet<T, EnumClass, NumValues>::operator/=(const Y rhs) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] /= rhs;
  }

  return *this;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>
EnumeratedScalarSet<T, EnumClass, NumValues>::operator+(const Y rhs) noexcept {
  return EnumeratedScalarSet<T, EnumClass, NumValues>(*this) += rhs;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>
EnumeratedScalarSet<T, EnumClass, NumValues>::operator-(const Y rhs) noexcept {
  return EnumeratedScalarSet<T, EnumClass, NumValues>(*this) -= rhs;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>
    EnumeratedScalarSet<T, EnumClass, NumValues>::
    operator*(const Y rhs) noexcept {
  return EnumeratedScalarSet<T, EnumClass, NumValues>(*this) *= rhs;
}

template <typename T, class EnumClass, int NumValues>
template <typename Y>
EnumeratedScalarSet<T, EnumClass, NumValues>
EnumeratedScalarSet<T, EnumClass, NumValues>::operator/(const Y rhs) noexcept {
  return EnumeratedScalarSet<T, EnumClass, NumValues>(*this) /= rhs;
}

template <typename T, class EnumClass, int NumValues>
void EnumeratedScalarSet<T, EnumClass, NumValues>::clampMin(T min) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] = std::max(stats[i], min);
  }
}

template <typename T, class EnumClass, int NumValues>
void EnumeratedScalarSet<T, EnumClass, NumValues>::clampMax(T max) noexcept {
  for (int i = 0; i < NumValues; i++) {
    stats[i] = std::min(stats[i], max);
  }
}

} // namespace stec

#endif // STEC_ENUMERATED_SCALAR_SET_HPP
