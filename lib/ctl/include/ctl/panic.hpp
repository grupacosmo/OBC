#ifndef CTL_PANIC_HPP
#define CTL_PANIC_HPP

#include <Arduino.h>

#include "source_location.hpp"

namespace ctl {

void panic(const char* msg, SourceLocation loc = SourceLocation::current());

}  // namespace ctl

#endif