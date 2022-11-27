#ifndef OBC_UTILS_HPP
#define OBC_UTILS_HPP

#include <type_traits>

namespace obc {

enum class Status { Debug, Flight };

template <typename Enum>
constexpr auto to_underlying(Enum e)
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

}  // namespace obc

#endif