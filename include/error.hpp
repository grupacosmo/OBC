#ifndef OBC_ERROR_HPP
#define OBC_ERROR_HPP

namespace obc {

enum class Errc { Busy };

inline void panic() { while (true) {} }

}  // namespace obc

#endif
