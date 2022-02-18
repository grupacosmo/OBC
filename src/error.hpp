#ifndef OBC_ERROR_HPP
#define OBC_ERROR_HPP

#include <ccl/source_location.hpp>

namespace obc {

enum class Errc { Busy };

void terminate_handler();
void panic_handler(const char* msg, ccl::SourceLocation loc);

}  // namespace obc

#endif
