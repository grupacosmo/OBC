#ifndef OBC_DIGITS_HPP
#define OBC_DIGITS_HPP

#include <Arduino.h>

namespace obc {

template <typename T>  // requires std::unsigned_integral<T>
constexpr bool has_tens_digit(T n)
{
    return n > 9;
}

template <typename T>  // requires std::unsigned_integral<T>
constexpr bool has_hundreds_digit(T n)
{
    return n > 99;
}

}  // namespace obc

#endif