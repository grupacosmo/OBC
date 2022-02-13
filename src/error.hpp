#ifndef OBC_ERROR_HPP
#define OBC_ERROR_HPP

#include <ctl/source_location.hpp>

namespace obc {

enum class Errc { Busy };

void terminate_handler();
void panic_handler(const char* msg, ctl::SourceLocation loc);

}  // namespace obc

#endif
