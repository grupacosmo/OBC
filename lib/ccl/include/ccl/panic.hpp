#ifndef CCL_PANIC_HPP
#define CCL_PANIC_HPP

#include "source_location.hpp"

namespace ccl {

using PanicHandler = void (*)(const char* msg, SourceLocation loc);

PanicHandler get_panic();
void set_panic(PanicHandler h);
void panic(const char* msg, SourceLocation loc = SourceLocation::current());

}  // namespace ccl

#endif