#ifndef OBC_FORMAT_HPP
#define OBC_FORMAT_HPP

// arduino framework conflict
#undef B1
#undef B2

#include <Arduino.h>
#include <fmt/format.h>

#include <iterator>

namespace obc {

namespace detail {

class StringBackInserter {
    String* s;

   public:
    using value_type = char;
    using reference = char&;
    using pointer = char*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::output_iterator_tag;

    explicit StringBackInserter(String& s) : s{&s} {}

    StringBackInserter& operator=(char c)
    {
        s->concat(c);
        return *this;
    }

    StringBackInserter& operator++() { return *this; }

    // NOLINTNEXTLINE(cert-dcl21-cpp)
    StringBackInserter& operator++(int) { return *this; }

    StringBackInserter& operator*() { return *this; }
};

StringBackInserter back_inserter(String& s) { return StringBackInserter{s}; }

}  // namespace detail

template <typename... T>
String format(fmt::format_string<T...> fmt, T&&... args)
{
    String buf;
    buf.reserve(fmt::formatted_size(fmt, args...));
    fmt::format_to(detail::back_inserter(buf), fmt, args...);
    return buf;
}

using fmt::format_to;
using fmt::format_to_n;
using fmt::formatted_size;

}  // namespace obc

#endif
