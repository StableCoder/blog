---
layout: page
title: Fixed Point Code
---

## Raw

- [fixed_point.hpp](fixed_point.hpp)

## Code

### fixed_point.hpp

<pre class="brush: cpp">
#include &lt;cmath>
#include &lt;cstdint>
#include &lt;limits>

/// \brief Allows high-precision storage of a fixed-point value.
/// \tparam T Basis type. Typically uint32_t.
/// \tparam Precision The number of precision points from the decimal.
template &lt;typename T, int8_t Precision>
class FixedPoint {
    static_assert(std::numeric_limits&lt;T>::is_exact,
                  "FixedPoint - Template parameter T must be an exact type type.");

  public:
    /// \brief Default constructor, sets all values to 0.
    FixedPoint();

    /// \brief Takes in a basic heap for the starting value
    /// \param initial_value The starting value
    template &lt;typename Y>
    FixedPoint(Y initial_value);

    /// \brief Takes in a value from a different heap of FixedPoint
    /// \param initial The starting value
    template &lt;typename Y, int8_t Z>
    FixedPoint(FixedPoint&lt;Y, Z> initial);

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

    template &lt;typename Y>
    FixedPoint &operator=(const Y);

    template &lt;typename Y>
    bool operator==(const Y &);

    template &lt;typename Y>
    bool operator!=(const Y &);

    template &lt;typename Y>
    bool operator&lt;(const Y &);

    template &lt;typename Y>
    bool operator>(const Y &);

    template &lt;typename Y>
    bool operator&lt;=(const Y &);

    template &lt;typename Y>
    bool operator>=(const Y &);

    template &lt;typename Y>
    FixedPoint &operator+=(const Y);

    template &lt;typename Y>
    FixedPoint &operator-=(const Y);

    template &lt;typename Y>
    FixedPoint &operator*=(const Y);

    template &lt;typename Y>
    FixedPoint &operator/=(const Y);

    template &lt;typename Y>
    FixedPoint operator+(const Y);

    template &lt;typename Y>
    FixedPoint operator-(const Y);

    template &lt;typename Y>
    FixedPoint operator*(const Y);

    template &lt;typename Y>
    FixedPoint operator/(const Y);

    bool operator==(const FixedPoint &);

    bool operator!=(const FixedPoint &);

    bool operator&lt;(const FixedPoint &);

    bool operator>(const FixedPoint &);

    bool operator&lt;=(const FixedPoint &);

    bool operator>=(const FixedPoint &);

    FixedPoint &operator+=(const FixedPoint &);

    FixedPoint &operator-=(const FixedPoint &);

    FixedPoint &operator*=(const FixedPoint &);

    FixedPoint &operator/=(const FixedPoint &);

    FixedPoint operator+(const FixedPoint &);

    FixedPoint operator-(const FixedPoint &);

    FixedPoint operator*(const FixedPoint &);

    FixedPoint operator/(const FixedPoint &);

    template &lt;typename Y, int8_t Z>
    FixedPoint &operator=(const FixedPoint&lt;Y, Z>);

    template &lt;typename Y, int8_t Z>
    bool operator==(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    bool operator!=(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    bool operator&lt;(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    bool operator>(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    bool operator&lt;=(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    bool operator>=(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    FixedPoint &operator+=(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    FixedPoint &operator-=(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    FixedPoint &operator*=(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    FixedPoint &operator/=(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    FixedPoint operator+(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    FixedPoint operator-(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    FixedPoint operator*(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y, int8_t Z>
    FixedPoint operator/(const FixedPoint&lt;Y, Z> &);

    template &lt;typename Y>
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

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision>::FixedPoint() : value(0) {}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision>::FixedPoint(Y initial_value) :
    value(initial_value * getPrecisionMultiplier()) {}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision>::FixedPoint(FixedPoint&lt;Y, Z> initial) : value(initial) {}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator=(const Y rhs) {
    value = rhs * getPrecisionMultiplier();

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
bool FixedPoint&lt;T, Precision>::operator==(const Y &rhs) {
    return value == rhs * getPrecisionMultiplier();
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
bool FixedPoint&lt;T, Precision>::operator!=(const Y &rhs) {
    return value != rhs * getPrecisionMultiplier();
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
bool FixedPoint&lt;T, Precision>::operator&lt;(const Y &rhs) {
    return value &lt; rhs * getPrecisionMultiplier();
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
bool FixedPoint&lt;T, Precision>::operator>(const Y &rhs) {
    return value > rhs * getPrecisionMultiplier();
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
bool FixedPoint&lt;T, Precision>::operator&lt;=(const Y &rhs) {
    return value &lt;= rhs * getPrecisionMultiplier();
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
bool FixedPoint&lt;T, Precision>::operator>=(const Y &rhs) {
    return value >= rhs * getPrecisionMultiplier();
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator+=(const Y rhs) {
    T test = rhs * getPrecisionMultiplier();
    value += test;

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator-=(const Y rhs) {
    value -= rhs * getPrecisionMultiplier();

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator*=(const Y rhs) {
    value *= rhs;

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator/=(const Y rhs) {
    value /= rhs;

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator+(const Y rhs) {
    return FixedPoint&lt;T, Precision>(*this) += rhs;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator-(const Y rhs) {
    return FixedPoint&lt;T, Precision>(*this) -= rhs;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator*(const Y rhs) {
    return FixedPoint&lt;T, Precision>(*this) *= rhs;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator/(const Y rhs) {
    return FixedPoint&lt;T, Precision>(*this) /= rhs;
}

template &lt;typename T, int8_t Precision>
bool FixedPoint&lt;T, Precision>::operator==(const FixedPoint &rhs) {
    return value == rhs.value;
}

template &lt;typename T, int8_t Precision>
bool FixedPoint&lt;T, Precision>::operator!=(const FixedPoint &rhs) {
    return value != rhs.value;
}

template &lt;typename T, int8_t Precision>
bool FixedPoint&lt;T, Precision>::operator&lt;(const FixedPoint &rhs) {
    return value &lt; rhs.value;
}

template &lt;typename T, int8_t Precision>
bool FixedPoint&lt;T, Precision>::operator>(const FixedPoint &rhs) {
    return value > rhs.value;
}

template &lt;typename T, int8_t Precision>
bool FixedPoint&lt;T, Precision>::operator&lt;=(const FixedPoint &rhs) {
    return value &lt;= rhs.value;
}

template &lt;typename T, int8_t Precision>
bool FixedPoint&lt;T, Precision>::operator>=(const FixedPoint &rhs) {
    return value >= rhs.value;
}

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator+=(const FixedPoint &rhs) {
    value += rhs.value;
    return *this;
}

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator-=(const FixedPoint &rhs) {
    value -= rhs.value;
    return *this;
}

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator*=(const FixedPoint &rhs) {
    value *= rhs.value;
    return *this;
}

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator/=(const FixedPoint &rhs) {
    value /= rhs.value;
    return *this;
}

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator+(const FixedPoint &rhs) {
    return FixedPoint(*this) += rhs;
}

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator-(const FixedPoint &rhs) {
    return FixedPoint(*this) -= rhs;
}

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator*(const FixedPoint &rhs) {
    return FixedPoint(*this) *= rhs;
}

template &lt;typename T, int8_t Precision>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator/(const FixedPoint &rhs) {
    return FixedPoint(*this) /= rhs;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator=(FixedPoint&lt;Y, Z> rhs) {
    if constexpr (Precision > Z) {
        value = static_cast&lt;T>(rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z));
    } else if constexpr (Precision &lt; Z) {
        value = static_cast&lt;T>(rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision));
    } else {
        value = static_cast&lt;T>(rhs.getRaw());
    }

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
bool FixedPoint&lt;T, Precision>::operator==(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value == static_cast&lt;T>(rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z));
    } else if constexpr (Precision &lt; Z) {
        return value == static_cast&lt;T>(rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision));
    } else {
        return value == static_cast&lt;T>(rhs.getRaw());
    }
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
bool FixedPoint&lt;T, Precision>::operator!=(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value != static_cast&lt;T>(rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z));
    } else if constexpr (Precision &lt; Z) {
        return value != static_cast&lt;T>(rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision));
    } else {
        return value != rhs.getRaw();
    }
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
bool FixedPoint&lt;T, Precision>::operator&lt;(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value &lt; static_cast&lt;T>(rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z));
    } else if constexpr (Precision &lt; Z) {
        return value &lt; static_cast&lt;T>(rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision));
    } else {
        return value &lt; static_cast&lt;T>(rhs.getRaw());
    }
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
bool FixedPoint&lt;T, Precision>::operator>(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value > static_cast&lt;T>(rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z));
    } else if constexpr (Precision &lt; Z) {
        return value > static_cast&lt;T>(rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision));
    } else {
        return value > static_cast&lt;T>(rhs.getRaw());
    }
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
bool FixedPoint&lt;T, Precision>::operator&lt;=(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value &lt;= static_cast&lt;T>(rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z));
    } else if constexpr (Precision &lt; Z) {
        return value &lt;= static_cast&lt;T>(rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision));
    } else {
        return value &lt;= static_cast&lt;T>(rhs.getRaw());
    }
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
bool FixedPoint&lt;T, Precision>::operator>=(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        return value >= static_cast&lt;T>(rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z));
    } else if constexpr (Precision &lt; Z) {
        return value >= static_cast&lt;T>(rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision));
    } else {
        return value >= static_cast&lt;T>(rhs.getRaw());
    }
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator+=(const FixedPoint&lt;Y, Z> &rhs) {
    // These, being templated, should collapse into a simple one-line function
    // during compilation.
    if constexpr (Precision > Z) {
        value += rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z);
    } else if constexpr (Precision &lt; Z) {
        value += rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision);
    } else {
        value += rhs.getRaw();
    }

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator-=(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        value -= rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z);
    } else if constexpr (Precision &lt; Z) {
        value -= rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision);
    } else {
        value -= rhs.getRaw();
    }

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator*=(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        value *= rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z);
    } else if constexpr (Precision &lt; Z) {
        value *= rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision);
    } else {
        value *= rhs.getRaw();
    }

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> &FixedPoint&lt;T, Precision>::operator/=(const FixedPoint&lt;Y, Z> &rhs) {
    if constexpr (Precision > Z) {
        value /= rhs.getRaw() * std::pow(static_cast&lt;T>(10), Precision - Z);
    } else if constexpr (Precision &lt; Z) {
        value /= rhs.getRaw() / std::pow(static_cast&lt;T>(10), Z - Precision);
    } else {
        value /= rhs.getRaw();
    }

    return *this;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator+(const FixedPoint&lt;Y, Z> &rhs) {
    return FixedPoint(*this) += rhs;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator-(const FixedPoint&lt;Y, Z> &rhs) {
    return FixedPoint(*this) -= rhs;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator*(const FixedPoint&lt;Y, Z> &rhs) {
    return FixedPoint(*this) *= rhs;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y, int8_t Z>
FixedPoint&lt;T, Precision> FixedPoint&lt;T, Precision>::operator/(const FixedPoint&lt;Y, Z> &rhs) {
    return FixedPoint(*this) /= rhs;
}

template &lt;typename T, int8_t Precision>
template &lt;typename Y>
FixedPoint&lt;T, Precision>::operator const Y() const {
    return static_cast&lt;Y>(value) / getPrecisionMultiplier();
}

template &lt;typename T, int8_t Precision>
const T FixedPoint&lt;T, Precision>::getRaw() const {
    return value;
}

template &lt;typename T, int8_t Precision>
constexpr int8_t FixedPoint&lt;T, Precision>::getPrecision() const {
    return Precision;
}

template &lt;typename T, int8_t Precision>
constexpr T FixedPoint&lt;T, Precision>::getPrecisionMultiplier() const {
    return std::pow(static_cast&lt;T>(10), Precision);
}

template &lt;typename T, int8_t Precision>
constexpr double FixedPoint&lt;T, Precision>::max() const {
    return static_cast&lt;double>(std::numeric_limits&lt;T>::max() / getPrecisionMultiplier());
}
</pre>