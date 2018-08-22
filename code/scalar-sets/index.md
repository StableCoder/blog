---
layout: page
title: Scalar Sets Example
---

## Raw

- [main.cpp](main.cpp)
- [scalar_set.hpp](scalar_set.hpp)

## Code

### main.cpp

<pre class="prettyprint">
#include "scalar_set.hpp"

#include &lt;iostream>
#include &lt;string>

enum class Special {
  Strength,
  Perception,
  Endurance,
  Charisma,
  Intelligence,
  Agility,
  Luck,
};
constexpr std::underlying_type&lt;Special>::type cNumSpecial =
    static_cast&lt;std::underlying_type&lt;Special>::type>(Special::Luck) + 1;

using SpecialSet = stec::EnumeratedScalarSet&lt;int8_t, Special, cNumSpecial>;
using SpecialSetf = stec::EnumeratedScalarSet&lt;float, Special, cNumSpecial>;

std::string to_string(SpecialSet special) {
  std::string retVal;
  retVal.reserve(100);

  retVal = "Strength " + std::to_string(special[Special::Strength]);
  retVal += "\nPerception " + std::to_string(special[Special::Perception]);
  retVal += "\nEndurance " + std::to_string(special[Special::Endurance]);
  retVal += "\nCharisma " + std::to_string(special[Special::Charisma]);
  retVal += "\nIntelligence " + std::to_string(special[Special::Intelligence]);
  retVal += "\nAgility " + std::to_string(special[Special::Agility]);
  retVal += "\nLuck " + std::to_string(special[Special::Luck]);
  retVal += '\n';

  return retVal;
}

int main() {
  SpecialSet base(5);
  SpecialSet perks, modifiers;
  SpecialSetf multiplier(1.f);

  base[Special::Intelligence] = 8;
  base[Special::Charisma] = 2;

  perks[Special::Perception] = -2;
  perks[Special::Strength] = 1;

  modifiers[Special::Luck] = 4;

  multiplier[Special::Charisma] = 2.5f;

  SpecialSet result = (base + perks + modifiers) * multiplier;

  std::cout &lt;&lt; "\nBase: \n"
            &lt;&lt; to_string(base) &lt;&lt; "\nPerks: \n"
            &lt;&lt; to_string(perks) &lt;&lt; "\nModifiers: \n"
            &lt;&lt; to_string(modifiers) &lt;&lt; "\nResult: \n"
            &lt;&lt; to_string(result) &lt;&lt; std::endl;
}
</pre>

### scalar_set.hpp

<pre class="prettyprint">
#include &lt;algorithm>
#include &lt;array>
#include &lt;cstdint>

/// \brief A template for use for tying together a bunch of scalar variables,
/// performing access with an enum class. 
/// \tparam T The underlying type of the template (ex int, float, etc.) 
/// \tparam EnumClass The enum type to use, must be zero-based and be in a solid 
/// incremental block. 
/// \tparam NumValues The number of values held in the template
///
/// The EnumeratedScalarSet is to make stat storage easier, where similar-type
/// stored scalar values can be stored and accessed either individually or the
/// total set of stats can be interacted on as a whole. Support also exists for
/// exchanging the stored values with StatTemplate types that have a different
/// scalar types, but must have the same EnumClass and NumValues.
///
/// This template also takes a parameter for an Enum class, allowing more
/// restricted/codified access to the elements.
template &lt;typename T, class EnumClass, int NumValues>
class EnumeratedScalarSet {
  static_assert(
      std::is_scalar&lt;T>::value,
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
  template &lt;typename Y>
  EnumeratedScalarSet(
      EnumeratedScalarSet&lt;Y, EnumClass, NumValues> initial) noexcept;

  /// \brief Returns a reference to the underlying value, denoted by the index.
  /// \return A reference to the value.
  T &operator[](const EnumClass) noexcept;

  /// \brief Returns a const reference to the underlying value, denoted by the
  /// index. \return A const reference to the value.
  T operator[](const EnumClass) const noexcept;

  template &lt;typename Y>
  bool operator==(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) const
      noexcept;

  template &lt;typename Y>
  bool operator!=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) const
      noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
  operator+=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
  operator-=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
  operator*=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
  operator/=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues>
  operator+(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues>
  operator-(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues>
  operator*(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &)noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues>
  operator/(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> &operator+=(const Y) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> &operator-=(const Y) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> &operator*=(const Y) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> &operator/=(const Y) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> operator+(const Y) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> operator-(const Y) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> operator*(const Y) noexcept;

  template &lt;typename Y>
  EnumeratedScalarSet&lt;T, EnumClass, NumValues> operator/(const Y) noexcept;

  /// \brief Clamps the minimum value of the internals to the given parameter.
  /// \param min The value that all values will be clamped to a minimum of.
  void clampMin(T min) noexcept;

  /// \brief Clamps the maximum value of internals to the given parameter.
  /// \param max The value that all values will be clamped to a maximum of.
  void clampMax(T max) noexcept;

private:
  /// The actual array of stored stat values.
  std::array&lt;T, NumValues> stats;
};

template &lt;typename T, class EnumClass, int NumValues>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::EnumeratedScalarSet(
    T initial) noexcept
    : stats{} {
  std::fill_n(stats.data(), NumValues, initial);
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::EnumeratedScalarSet(
    EnumeratedScalarSet&lt;Y, EnumClass, NumValues> initial) noexcept
    : stats{} {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] = initial[static_cast&lt;EnumClass>(i)];
  }
}

template &lt;typename T, class EnumClass, int NumValues>
T &EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator[](const EnumClass rhs) noexcept {
  return stats[static_cast&lt;typename std::underlying_type&lt;EnumClass>::type>(
      rhs)];
}

template &lt;typename T, class EnumClass, int NumValues>
T EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator[](const EnumClass rhs) const noexcept {
  return stats[static_cast&lt;typename std::underlying_type&lt;EnumClass>::type>(
      rhs)];
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
bool EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator==(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) const
    noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    if (stats[i] != rhs[static_cast&lt;EnumClass>(i)]) {
      return false;
    }
  }

  return true;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
bool EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator!=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) const
    noexcept {
  return !(*this == rhs);
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator+=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] += rhs[static_cast&lt;EnumClass>(i)];
  }

  return *this;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator-=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] -= rhs[static_cast&lt;EnumClass>(i)];
  }

  return *this;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator*=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] *= rhs[static_cast&lt;EnumClass>(i)];
  }

  return *this;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator/=(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] /= rhs[static_cast&lt;EnumClass>(i)];
  }

  return *this;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator+(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) noexcept {
  return EnumeratedScalarSet&lt;T, EnumClass, NumValues>(*this) += rhs;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator-(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) noexcept {
  return EnumeratedScalarSet&lt;T, EnumClass, NumValues>(*this) -= rhs;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
    EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator*(
        const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) noexcept {
  return EnumeratedScalarSet&lt;T, EnumClass, NumValues>(*this) *= rhs;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
operator/(const EnumeratedScalarSet&lt;Y, EnumClass, NumValues> &rhs) noexcept {
  return EnumeratedScalarSet&lt;T, EnumClass, NumValues>(*this) /= rhs;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator+=(const Y rhs) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] += rhs;
  }

  return *this;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator-=(const Y rhs) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] -= rhs;
  }

  return *this;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator*=(const Y rhs) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] *= rhs;
  }

  return *this;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues> &
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator/=(const Y rhs) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] /= rhs;
  }

  return *this;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator+(const Y rhs) noexcept {
  return EnumeratedScalarSet&lt;T, EnumClass, NumValues>(*this) += rhs;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator-(const Y rhs) noexcept {
  return EnumeratedScalarSet&lt;T, EnumClass, NumValues>(*this) -= rhs;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
    EnumeratedScalarSet&lt;T, EnumClass, NumValues>::
    operator*(const Y rhs) noexcept {
  return EnumeratedScalarSet&lt;T, EnumClass, NumValues>(*this) *= rhs;
}

template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator/(const Y rhs) noexcept {
  return EnumeratedScalarSet&lt;T, EnumClass, NumValues>(*this) /= rhs;
}

template &lt;typename T, class EnumClass, int NumValues>
void EnumeratedScalarSet&lt;T, EnumClass, NumValues>::clampMin(T min) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] = std::max(stats[i], min);
  }
}

template &lt;typename T, class EnumClass, int NumValues>
void EnumeratedScalarSet&lt;T, EnumClass, NumValues>::clampMax(T max) noexcept {
  for (int i = 0; i &lt; NumValues; i++) {
    stats[i] = std::min(stats[i], max);
  }
}
</pre>