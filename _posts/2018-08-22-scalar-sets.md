---
layout: post
title: Collecting scalar values together in C++11
brief: Making life easier through enum flags, structs and offsets.
date: 2018-08-22 00:00:00 -0000
tags : c++, software
summary: There are times when putting together a set of like scalar values can be useful, without having to explictly create them in a class.
---

Typically, these values are denominations of the same idea or principle, and where operations should be accomplished on either the individual parts, or the whole set together. As an example, in many RPGs are many sets of numerical values that represent traits. For example in Fallout characters have basic stats represented by the 'SPECIAL' system. In a more traditional setting, this would be represented either as an array, where the values are selected by offsets, or a class where the variables are individually named:

<pre class="prettyprint">
enum {
    eStrength,
    ePerception,
    eEndurance,
    eCharisma,
    eAgility,
    eLuck,
};

struct Special1 {
    int values[7];

    // Accessors
    int strength() const noexcept { return values[0]; }
    void strength(int val) noexcept { values[0] = val; }
    int perception() const noexcept { return values[1]; }
    // ...

    // Use like `getVal(eStrength);`
    int getVal(int flag) const noexcept { return values[flag]; }
    // Use like `setVal(ePerception, 9);`
    void setVal(int flag, int val) noexcept { values[flag] = val; }
};

// or

struct Special2 {
    int strength;
    int perception;
    int endurance;
    int charisma;
    int agility;
    int luck;
};
</pre>

Both approaches are viable, where they can have their capabilities enhanced by adding a whole bunch of accessor or specialized operator functions into the mix.

`Special1` can easily be accessed by using the offset of the type one wishes to access, which can also be changed to be an enum that represents the corresponding offset values. This is pretty easy to implement internal functions for operating on the class, as one only needs to write functions that operateThis is fairly easy to use in other systems that work on the class, as a common flag can be changed out easily for pretty generic code. However, there's no real guard against accidentally using the wrong flag or offset.

`Special2` alleviates the issue of accidentally working on the wrong variable as all items are explicitly created and named, being generally safer. It can be modified by a operator[] to use a flag, but this then leads into the issues of unsafeness the first struct has. 

For either, to perform struct-wide operations requires specialized functions for these structs, and need to be remade for each like-type. For example in Fallout would be to also reprsent items such as the actor's skills, or items such as health, armor class, etc. To boot would also be if there's a requirement for the values to be of other types than int, such as float. Basically, either is a bit of a maintenance nightmare.

This is a fair amount of boilerplate code, and improvements to one struct's inner workings may not carry over without some developer legwork.

With the use of templates, one can have the best of all worlds, generic code using strict enum class flags, and classes that autogenerate functions that operate on the whole set together.

<pre class="prettyprint">
/// \brief A template for use for tying together a bunch of scalar variables, performing access with
/// an enum class.
/// \tparam T The underlying type of the template (ex int, float, etc.)
/// \tparam EnumClass The enum type to use, must be zero-based and be in a solid incremental block.
/// \tparam NumValues The number of values held in the template
template &lt;T, EnumClass, NumValues>
class ScalarSet{
    private:
    static_assert(std::is_scalar&lt;T>::value,
              "ScalarSet - Template parameter T must be a scalar type.");

    std::array&lt;T, NumValues> values;

    public:
    ...
</pre>

Most functions internally can be implemented trivially when operating across an array, as it just requires iterating over the internal array.
<pre class="prettyprint">
ScalarSet(T init = 0) : stats{} {
    std::fill_n(stats.data(), NumValues, init);
}

/// \brief Adds the individual elements of a *similar* same-type scalar set.
template &lt;typename T, class EnumClass, int NumValues>
template &lt;typename Y>
EnumeratedScalarSet&lt;T, EnumClass, NumValues>
    &EnumeratedScalarSet&lt;T, EnumClass, NumValues>::operator+=(
        EnumeratedScalarSet&lt;Y, EnumClass, NumValues> const &rhs) noexcept {
    for (int i = 0; i < NumValues; i++) {
        stats[i] += rhs[static_cast&lt;EnumClass>(i)];
    }

    return *this;
}

/// \brief Adds a scalar value to all elements and returns a reference.
template &lt;typename Y>
ScalarSet &operator+=(const Y rhs) noexcept{
    for (int i = 0; i < NumValues; i++) {
        stats[i] += rhs;
    }

    return *this;
}
</pre>

And access can be restricted to a easy-to-use enum class flag set instead of individually implemented accessors through an operator[]:
<pre class="prettyprint">
T &operator[](const EnumClass rhs) noexcept {
    return stats[static_cast&lt;typename std::underlying_type&lt;EnumClass>::type>(rhs)];
}
</pre>

Filling out the rest of the function's arithmetic operators would then net an exquisite class that can be shrunk, expanded, and reused quite easily (via the associated flags), with both operations on individual values and the whole set together without issue.

<pre class="prettyprint">
enum class Special {
    Strength,
    Perception,
    Endurance,
    Charisma,
    Intelligence,
    Agility,
    Luck,
};
constexpr auto cNumSpecial = 7;

using SpecialSet = ScalarSet&lt;int8_t, Special, cNumSpecial>;
using SpecialSetf = ScalarSet&lt;float, Special, cNumSpecial>;

int main(int argc, char **argv) {
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

    print(result);

    return 0;
}
</pre>
This is simple, easy to read, and fully safe. Trying to throw in the incorrect flag type would spew out a compiler error, and otherwise the flags and accessors are shared across the types easily.

Throwing that through would produce the correct and desired results of:
```
Result:
Strength 6
Perception 3
Endurance 5
Charisma 5
Intelligence 8
Agility 5
Luck 9
```

The full example can be found [here](/assets/posts/scalar-sets/scalar-fallout.zip).