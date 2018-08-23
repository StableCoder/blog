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

#ifndef STEC_FIXED_POINT_HPP_INCLUDED
#define STEC_FIXED_POINT_HPP_INCLUDED

#include <cmath>
#include <cstdint>
#include <limits>

namespace stec {

/// \brief Allows high-precision storage of a fixed-point value.
/// \tparam T Basis type. Typically uint32_t.
/// \tparam Precision The number of precision points from the decimal.
///
/// A template class used to store numbers of specific precision that can use non-floating point
/// types, such as int, unsigned, etc.
///
/// Whilst not normally of great use, there are some usecases where the lower precision limit allows
/// for greater flexibility in use for larger ranges while maintainign greate precision.
///
/// The below is an example run from the test_fixed_point_info program from the tests/ folder, built
/// with GCC 6.3.1 and run on Windows 10. The program started at 0 for each value, and incremented
/// by the given value of that presision until the maximum changable value was achieved.
///
/// | Increment | Float Max | Fixed Max (uint32_t) | Fixed Max (int32_t)|
/// |:--------- |:--------- |:-------------------- |:------------------ |
/// | 0.1       | 2097152   | 429496729.5          | 214748364.7        |
/// | 0.01      | 262144    | 42949672.95          | 21474836.47        |
/// | 0.001     | 32768     | 4294967.295          | 2147483.647        |
/// | 0.0001    | 2048      | 429496.7295          | 214748.3647        |
/// | 0.00001   | 256       | 42949.67295          | 21474.83647        |
///
/// As can be seen above, the IEEE float can only increment at the given precision to a certain
/// point, before the floating point error basically error out the increment to 0 and stops
/// increasing. The FixedPoint items can, however, keep incrementing until reading the max of the
/// given internal value.
///
/// It should be noted that using double types basically makes this moot. However, this allows for
/// much better/larger ranges using only 4 byte values still, with the given caveat that the
/// precision is fixed.
template <typename T, int8_t Precision>
class FixedPoint {
    static_assert(std::numeric_limits<T>::is_exact,
                  "FixedPoint - Template parameter T must be an exact type type.");

  public:
    /// \brief Default constructor, sets all values to 0.
    FixedPoint();

    /// \brief Takes in a basic heap for the starting value
    /// \param initial_value The starting value
    template <typename Y>
    FixedPoint(Y initial_value);

    /// \brief Takes in a value from a different heap of FixedPoint
    /// \param initial The starting value
    template <typename Y, int8_t Z>
    FixedPoint(FixedPoint<Y, Z> initial);

    /// \brief Destructor
    ~FixedPoint() = default;

    /// \brief Copy Constructor
    FixedPoint(const FixedPoint &) = default;

    /// \brief Copy Operator=
    FixedPoint &operator=(const FixedPoint &) = default;

    /// \brief Move Constructor
    FixedPoint(FixedPoint &&) noexcept = default;

    /// \brief Move Operator
    FixedPoint &operator=(FixedPoint &&) noexcept = default;

    template <typename Y>
    FixedPoint &operator=(const Y);

    template <typename Y>
    bool operator==(const Y &);

    template <typename Y>
    bool operator!=(const Y &);

    template <typename Y>
    bool operator<(const Y &);

    template <typename Y>
    bool operator>(const Y &);

    template <typename Y>
    bool operator<=(const Y &);

    template <typename Y>
    bool operator>=(const Y &);

    template <typename Y>
    FixedPoint &operator+=(const Y);

    template <typename Y>
    FixedPoint &operator-=(const Y);

    template <typename Y>
    FixedPoint &operator*=(const Y);

    template <typename Y>
    FixedPoint &operator/=(const Y);

    template <typename Y>
    FixedPoint operator+(const Y);

    template <typename Y>
    FixedPoint operator-(const Y);

    template <typename Y>
    FixedPoint operator*(const Y);

    template <typename Y>
    FixedPoint operator/(const Y);

    bool operator==(const FixedPoint &);

    bool operator!=(const FixedPoint &);

    bool operator<(const FixedPoint &);

    bool operator>(const FixedPoint &);

    bool operator<=(const FixedPoint &);

    bool operator>=(const FixedPoint &);

    FixedPoint &operator+=(const FixedPoint &);

    FixedPoint &operator-=(const FixedPoint &);

    FixedPoint &operator*=(const FixedPoint &);

    FixedPoint &operator/=(const FixedPoint &);

    FixedPoint operator+(const FixedPoint &);

    FixedPoint operator-(const FixedPoint &);

    FixedPoint operator*(const FixedPoint &);

    FixedPoint operator/(const FixedPoint &);

    template <typename Y, int8_t Z>
    FixedPoint &operator=(const FixedPoint<Y, Z>);

    template <typename Y, int8_t Z>
    bool operator==(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    bool operator!=(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    bool operator<(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    bool operator>(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    bool operator<=(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    bool operator>=(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    FixedPoint &operator+=(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    FixedPoint &operator-=(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    FixedPoint &operator*=(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    FixedPoint &operator/=(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    FixedPoint operator+(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    FixedPoint operator-(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    FixedPoint operator*(const FixedPoint<Y, Z> &);

    template <typename Y, int8_t Z>
    FixedPoint operator/(const FixedPoint<Y, Z> &);

    template <typename Y>
    explicit operator const Y() const;

    /// \brief Get the raw, underlying value.
    const T getRaw() const;

    /// \brief Returns the number of precision digits of the class.
    constexpr int8_t getPrecision() const;

    /// \brief Returns the multiplier applied to the stored values.
    constexpr T getPrecisionMultiplier() const;

    /// \brief Returns the maximum value that can be stored in the class.
    constexpr double max() const;

  private:
    /// The actual, underlying value.
    T value;
};

template <typename T, int8_t Precision>
FixedPoint<T, Precision>::FixedPoint() : value(0) {}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision>::FixedPoint(Y initial_value) :
    value(initial_value * getPrecisionMultiplier()) {}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision>::FixedPoint(FixedPoint<Y, Z> initial) : value(initial) {}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator=(const Y rhs) {
    value = rhs * getPrecisionMultiplier();

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y>
bool FixedPoint<T, Precision>::operator==(const Y &rhs) {
    return value == rhs * getPrecisionMultiplier();
}

template <typename T, int8_t Precision>
template <typename Y>
bool FixedPoint<T, Precision>::operator!=(const Y &rhs) {
    return value != rhs * getPrecisionMultiplier();
}

template <typename T, int8_t Precision>
template <typename Y>
bool FixedPoint<T, Precision>::operator<(const Y &rhs) {
    return value < rhs * getPrecisionMultiplier();
}

template <typename T, int8_t Precision>
template <typename Y>
bool FixedPoint<T, Precision>::operator>(const Y &rhs) {
    return value > rhs * getPrecisionMultiplier();
}

template <typename T, int8_t Precision>
template <typename Y>
bool FixedPoint<T, Precision>::operator<=(const Y &rhs) {
    return value <= rhs * getPrecisionMultiplier();
}

template <typename T, int8_t Precision>
template <typename Y>
bool FixedPoint<T, Precision>::operator>=(const Y &rhs) {
    return value >= rhs * getPrecisionMultiplier();
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator+=(const Y rhs) {
    T test = rhs * getPrecisionMultiplier();
    value += test;

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator-=(const Y rhs) {
    value -= rhs * getPrecisionMultiplier();

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator*=(const Y rhs) {
    value *= rhs;

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator/=(const Y rhs) {
    value /= rhs;

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator+(const Y rhs) {
    return FixedPoint<T, Precision>(*this) += rhs;
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator-(const Y rhs) {
    return FixedPoint<T, Precision>(*this) -= rhs;
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator*(const Y rhs) {
    return FixedPoint<T, Precision>(*this) *= rhs;
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator/(const Y rhs) {
    return FixedPoint<T, Precision>(*this) /= rhs;
}

template <typename T, int8_t Precision>
bool FixedPoint<T, Precision>::operator==(const FixedPoint &rhs) {
    return value == rhs.value;
}

template <typename T, int8_t Precision>
bool FixedPoint<T, Precision>::operator!=(const FixedPoint &rhs) {
    return value != rhs.value;
}

template <typename T, int8_t Precision>
bool FixedPoint<T, Precision>::operator<(const FixedPoint &rhs) {
    return value < rhs.value;
}

template <typename T, int8_t Precision>
bool FixedPoint<T, Precision>::operator>(const FixedPoint &rhs) {
    return value > rhs.value;
}

template <typename T, int8_t Precision>
bool FixedPoint<T, Precision>::operator<=(const FixedPoint &rhs) {
    return value <= rhs.value;
}

template <typename T, int8_t Precision>
bool FixedPoint<T, Precision>::operator>=(const FixedPoint &rhs) {
    return value >= rhs.value;
}

template <typename T, int8_t Precision>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator+=(const FixedPoint &rhs) {
    value += rhs.value;
    return *this;
}

template <typename T, int8_t Precision>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator-=(const FixedPoint &rhs) {
    value -= rhs.value;
    return *this;
}

template <typename T, int8_t Precision>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator*=(const FixedPoint &rhs) {
    value *= rhs.value;
    return *this;
}

template <typename T, int8_t Precision>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator/=(const FixedPoint &rhs) {
    value /= rhs.value;
    return *this;
}

template <typename T, int8_t Precision>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator+(const FixedPoint &rhs) {
    return FixedPoint(*this) += rhs;
}

template <typename T, int8_t Precision>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator-(const FixedPoint &rhs) {
    return FixedPoint(*this) -= rhs;
}

template <typename T, int8_t Precision>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator*(const FixedPoint &rhs) {
    return FixedPoint(*this) *= rhs;
}

template <typename T, int8_t Precision>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator/(const FixedPoint &rhs) {
    return FixedPoint(*this) /= rhs;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator=(FixedPoint<Y, Z> rhs) {
    if constexpr (Precision > Z) {
        value = static_cast<T>(rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z));
    } else if constexpr (Precision < Z) {
        value = static_cast<T>(rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision));
    } else {
        value = static_cast<T>(rhs.getRaw());
    }

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
bool FixedPoint<T, Precision>::operator==(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value == static_cast<T>(rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z));
    } else if constexpr (Precision < Z) {
        return value == static_cast<T>(rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision));
    } else {
        return value == static_cast<T>(rhs.getRaw());
    }
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
bool FixedPoint<T, Precision>::operator!=(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value != static_cast<T>(rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z));
    } else if constexpr (Precision < Z) {
        return value != static_cast<T>(rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision));
    } else {
        return value != rhs.getRaw();
    }
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
bool FixedPoint<T, Precision>::operator<(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value < static_cast<T>(rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z));
    } else if constexpr (Precision < Z) {
        return value < static_cast<T>(rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision));
    } else {
        return value < static_cast<T>(rhs.getRaw());
    }
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
bool FixedPoint<T, Precision>::operator>(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value > static_cast<T>(rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z));
    } else if constexpr (Precision < Z) {
        return value > static_cast<T>(rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision));
    } else {
        return value > static_cast<T>(rhs.getRaw());
    }
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
bool FixedPoint<T, Precision>::operator<=(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value <= static_cast<T>(rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z));
    } else if constexpr (Precision < Z) {
        return value <= static_cast<T>(rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision));
    } else {
        return value <= static_cast<T>(rhs.getRaw());
    }
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
bool FixedPoint<T, Precision>::operator>=(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value >= static_cast<T>(rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z));
    } else if constexpr (Precision < Z) {
        return value >= static_cast<T>(rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision));
    } else {
        return value >= static_cast<T>(rhs.getRaw());
    }
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator+=(const FixedPoint<Y, Z> &rhs) {
    // These, being templated, should collapse into a simple one-line function
    // during compilation.
    if constexpr (Precision > Z) {
        value += rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z);
    } else if constexpr (Precision < Z) {
        value += rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision);
    } else {
        value += rhs.getRaw();
    }

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator-=(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        value -= rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z);
    } else if constexpr (Precision < Z) {
        value -= rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision);
    } else {
        value -= rhs.getRaw();
    }

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator*=(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        value *= rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z);
    } else if constexpr (Precision < Z) {
        value *= rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision);
    } else {
        value *= rhs.getRaw();
    }

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> &FixedPoint<T, Precision>::operator/=(const FixedPoint<Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        value /= rhs.getRaw() * std::pow(static_cast<T>(10), Precision - Z);
    } else if constexpr (Precision < Z) {
        value /= rhs.getRaw() / std::pow(static_cast<T>(10), Z - Precision);
    } else {
        value /= rhs.getRaw();
    }

    return *this;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator+(const FixedPoint<Y, Z> &rhs) {
    return FixedPoint(*this) += rhs;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator-(const FixedPoint<Y, Z> &rhs) {
    return FixedPoint(*this) -= rhs;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator*(const FixedPoint<Y, Z> &rhs) {
    return FixedPoint(*this) *= rhs;
}

template <typename T, int8_t Precision>
template <typename Y, int8_t Z>
FixedPoint<T, Precision> FixedPoint<T, Precision>::operator/(const FixedPoint<Y, Z> &rhs) {
    return FixedPoint(*this) /= rhs;
}

template <typename T, int8_t Precision>
template <typename Y>
FixedPoint<T, Precision>::operator const Y() const {
    return static_cast<Y>(value) / getPrecisionMultiplier();
}

template <typename T, int8_t Precision>
const T FixedPoint<T, Precision>::getRaw() const {
    return value;
}

template <typename T, int8_t Precision>
constexpr int8_t FixedPoint<T, Precision>::getPrecision() const {
    return Precision;
}

template <typename T, int8_t Precision>
constexpr T FixedPoint<T, Precision>::getPrecisionMultiplier() const {
    return std::pow(static_cast<T>(10), Precision);
}

template <typename T, int8_t Precision>
constexpr double FixedPoint<T, Precision>::max() const {
    return static_cast<double>(std::numeric_limits<T>::max() / getPrecisionMultiplier());
}

} // namespace stec

#endif // STEC_FIXED_POINT_HPP_INCLUDED