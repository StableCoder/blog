---
layout: post
title: Fixed Point Precision Type
brief: Because sometimes floats are not enough.
date: 2017-06-25 21:23 UTC
tags: c++, stec, software
summary: One of the most valuable basic types in computer science is the floating point type. The float is that allows for a massive range of storable values, at the cost of some precision after the value becomes large enough, due to the fact that floats can only store ~7.2 decimal digits. Thus, if we wish to keep a certain fixed amount or precision, we need to either put coded limits around the use of particular floats, or just create our own fixed point type.
---

Fixed point basically means that the divisor, or decimal point, is fixed, and can't move. The most well known type is of course, the basic char, int, long, etc. where they have a fixed point where only whole numbers can be used with them. Thus, if one is so inclined, one can just use one of those types and make sure each interaction is divided by a certain value, such as ten, hundreds, or thousandths. Of course, in computers we're usually a bit lazy, so instead one can create an automated type that does this automatically.

But, to start with, what are we trying to fix? This is simple, and shown below:

| Increment | Float Max | Fixed Max (uint32_t) | Fixed Max (int32_t) |
| --------- | --------- | -------------------- | ------------------- |
| 0.1       | 2097152   | 429496729.5          | 214748364.7         |
| 0.01      | 262144    | 42949672.95          | 21474836.47         |
| 0.001     | 32768     | 4294967.295          | 2147483.647         |
| 0.0001    | 2048      | 429496.7295          | 214748.3647         |
| 0.00001   | 256       | 42949.67295          | 21474.83647         |

As one could deduce, the precision of floats when dealing with tenths, hundredths, and thousandths only goes so far, even to hundred-thousandths where the max attainable accurate value is ~256, whereas using a fixed_point type, of signed int can take us accurately up to ~21474, This is a fair increase in accurate keeping, so long as we want a max precision of 5 digits and accept the upper limit.

So, to start a fixed point class, we need a templated object, that allows us to specify the underlying type, and the number of digits of precision:

<pre class="prettyprint">
template &lt;typename T, uint8_t Precision>
class fixed_point
{
private:
    T m_Value;
    
    ...
</pre>

Next, we require a large number of overloade constructors and operators to perform arithmetic with both basic types and other variants of fixed_point templates:

<pre class="prettyprint">
    ...
    
public:
    ///Constructors && Destructor
    // Regular, sets starting value to 0.
    fixed_point();
    // Takes in a basic type for the starting value
    template &lt;typename Y>
    fixed_point(Y p_InitialValue);
    // Takes in a value from a different type of fixed_point
    template &lt;typename Y, uint8_t Z>
    fixed_point(fixed_point&lt;Y, Z> p_Initial);
    // Destructor
    ~fixed_point();
    
    ///&lt; Basic-Type Operators
    // Arithmetic with basic types
    template &lt;typename Y>
    fixed_point&lt;T, Precision>& operator+=(const Y);
    template &lt;typename Y>
    fixed_point&lt;T, Precision>& operator-=(const Y);
        etc...

    ///&lt; Same-Type Operators
    // Arithmetic with same-type of fixed_point
    fixed_point&lt;T, Precision>& operator+=(const fixed_point&);
    fixed_point&lt;T, Precision>& operator-=(const fixed_point&);
        etc...

    ///&lt; Different-Type Operators
    // Arithmetic with different variants of fixed_point
    template &lt;typename Y, uint8_t Z>
    fixed_point&lt;T, Precision>& operator+=(const fixed_point&lt;Y, Z>&);
    template &lt;typename Y, uint8_t Z>
    fixed_point&lt;T, Precision>& operator-=(const fixed_point&lt;Y, Z>&);
        etc...
    
    ...
</pre>

Finally, we require the ability to convert the value stored to all the regular basic types, and the ability to determine basic stats of the type:


<pre class="prettyprint">
    ...

    ///&lt; Other Functions
    // Get the value, type is determined by template
    template &lt;typename Y>
    operator const Y() const;

    // Retrieve basic stats of the type
    constexpr uint8_t getPrecision() const;
    constexpr T getPrecisionMultiplier() const;
    constexpr double max() const;
}
</pre>

However, a large point of trouble is, of course, interacting with other template instantiations of the fixed_point class, most importantly when the precision between two types is different. For these types, there is a more involved, but still simplistic code behind.


<pre class="prettyprint">
fixed_point&lt;T, Precision>& operator+=(const fixed_point&lt;Y, Z>& rhs)
{
    // These, being templated, should collapse into a simple one-line function
    // during compilation.
    if(Precision > Z)
    {
        constexpr T multiplier = pow((T)10, Precision - Z);
        m_Value += rhs.getRaw() * multiplier;
    }
    else if(Precision &lt; Z)
    {
        constexpr T multiplier = pow((T)10, Z - Precision);
        m_Value += rhs.getRaw() / multiplier;
    }
    else
    {
        m_Value += rhs.getRaw();
    }

    return *this;
}
</pre>

The code will, for each template variant, collapse down to a single operation, because first of all, the Precision and Z are fixed as template parameters, thus only one of the three code paths will always be taken, thus the compiler will optimize out the unused ones. Secondly, the multiplier, being a constexpr value, will be evaluated at compile time, such that no actual calculations are done for determining the multiplier during runtime. Thus, typically, the top operation will essentially turn into this:

<pre class="prettyprint">
fixed_point&lt;T, Precision>& operator+=(const fixed_point&lt;Y, Z>& rhs)
{
    m_Value += rhs.getRaw() %op% %compile-time-constant%;
    
    return *this;
</pre>

With all of the overloaded basic arithmetic operators, the fixed_point class type can interact with all the other types, and have arithmetic performed with little effort required on the part of the developer, like so:

<pre class="prettyprint">
    fixed_point&lt;int32_t, 2> test;
    fixed_point&lt;uint32_t, 3> test2;

    test += 1.0f;
    test -= (double)0.1;
    test = test + 500;
    test = 500 / (int)test;

    test = test2;
    test2 = test;
    test == test2;
    test2 == test;
    test &lt; test2;
    test2 &lt; test;
</pre>

In the end, of course, what have we gained? Well, we have a new easy-to-use type that allows for a larger range of accuracy than floats, provided you know the precision you want to keep, and that it doesn't overflow the known maximum, which are both easily accessible. Not only that, but the fixed_point doesn't suffer from the floating point precision issues, it's a fully round (divided) value, and can hold more digits because there's no bits taken up by the exponent.

Basically, the whole thing is a more accurate, but range limited type, that takes the same amount of memory. On some architecture types, even may operate faster.